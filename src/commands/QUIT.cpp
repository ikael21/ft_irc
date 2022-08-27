#include "commands.hpp"


void assign_new_operator(irc::IrcServer& server,
                         irc::User& user,
                         irc::IrcServer::t_channel_list::iterator ch) {

  if (ch->get_users().size() > 1 && ch->get_num_opers() == 1 && ch->is_oper(user)) {
    irc::User* next_oper = ch->get_users()[1];

    std::string mode_command =  "MODE " + ch->get_name() + " +o " + next_oper->get_nick();
    irc::Command(server, user, mode_command).execute();
  }
}


void QUIT(irc::Command* command) {

  irc::User& user = command->get_user();

  std::string prefix_quit_msg = user.get_prefix_msg() + "PART ";
  std::string suffix_quit_msg;

  if (command->num_args())
    suffix_quit_msg = (command->get_arguments()[0][0] == ':' ? " " : " :") + command->get_arguments()[0];

  irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();
  irc::IrcServer::t_channel_list::iterator ch = channels.begin();

  while (ch != channels.end()) {
    if (ch->user_on_channel(user)) {

      assign_new_operator(command->get_server(), user, ch);

      std::string full_msg = prefix_quit_msg + ch->get_name() + suffix_quit_msg;
      ch->send_to_channel(user, full_msg, false);
      ch->remove_user(user);
    }
    ch = ch->is_empty() ? channels.erase(ch) : ++ch;
  }
  shutdown(user.get_fd(), SHUT_WR);
  user.get_buffer().clear();
}
