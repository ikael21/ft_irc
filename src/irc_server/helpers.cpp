#include "IrcServer.hpp"
#include "Command.hpp"

irc::User& irc::IrcServer::_find_or_create_user(int fd) {
  for (t_userlist::iterator i = _users.begin(); i != _users.end(); ++i) {
    if (i->get_fd() == fd)
      return *i;
  }
  _users.push_back(User(fd));
  return _users.back();
}


void irc::IrcServer::_ping_client(User& user) {
  _ping_by_nickname(user);
  user.set_state(WAIT_PONG);
  user.set_last_activity(time(NULL));
  #ifdef DEBUG
    _print_user_state(user);
  #endif
}


void irc::IrcServer::_ping_by_nickname(const User& user) {
  std::string message("PING " + user.get_nick() + END_OF_MESSAGE);
  send(user.get_fd(), message.c_str(), message.length(), 0);
  #ifdef DEBUG
    _print_message_for_user(user, message);
  #endif
}


void irc::IrcServer::_remove_user_from_channels(User& user) {

  irc::IrcServer::t_channel_list::iterator ch = _channels.begin();
  std::string prefix_quit_msg = user.get_prefix_msg() + "PART ";

  while (ch != _channels.end()) {
    if (ch->user_on_channel(user)) {

      // assign new operator
      if (ch->get_users().size() > 1 && ch->get_num_opers() == 1 && ch->is_oper(user)) {
        irc::User* next_oper = ch->get_users()[1];

        std::string mode_command =  "MODE " + ch->get_name() + " +o " + next_oper->get_nick();
        irc::Command(*this, user, mode_command).execute();
      }

      std::string full_msg = prefix_quit_msg + ch->get_name();
      ch->send_to_channel(user, full_msg, false);
      ch->remove_user(user);
    }
    ch = ch->is_empty() ? _channels.erase(ch) : ++ch;
  }
}
