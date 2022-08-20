#include "commands.hpp"


void check_and_return_names(Command *command, irc::IrcServer::t_channel_list::iterator& ch) {

  if(!ch->is_private() || \
    ((ch->is_private() || ch->is_secret()) && ch->user_on_channel(command->get_user()))) {

    std::vector<User*> channel_users = ch->get_users();
    std::stringstream users_str;

    for (size_t i = 0; i < channel_users.size(); ++i) {
      if (channel_users[i]->is_invisible())
        continue;
      users_str << (ch->is_oper(*channel_users[i]) ? "@" : "+") << channel_users[i]->get_nick() << " ";
    }

    if (channel_users.size())
      command->reply(RPL_NAMREPLY, ch->get_name(), users_str.str());
    command->reply(RPL_ENDOFNAMES, ch->get_name());
  }
}


void NAMES(Command *command) {

  std::vector<std::string> args = command->get_arguments();
  irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();

  if (args.size()) {

    std::vector<std::string> channel_list = split(args[0], ',');

    for (size_t i = 0; i < channel_list.size(); ++i) {
      irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channel_list[i]);
      if (ch != channels.end()) {
        check_and_return_names(command, ch);
      }
    }
  } else {

    // print all users in all channels
    for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
      check_and_return_names(command, ch);
    }

    // print all users, that not on channels
    irc::IrcServer::t_userlist& users = command->get_server().get_users();
    std::stringstream s_users_not_on_channels;

    for (irc::IrcServer::t_userlist::iterator usr = users.begin(); usr != users.end(); ++usr) {
      if (usr->is_invisible())
        continue;

      bool not_on_channels = true;
      for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
        if (ch->user_on_channel(*usr)) {
          not_on_channels = false;
          break;
        }
      }
      if (not_on_channels)
        s_users_not_on_channels << usr->get_nick() << " ";
    }

    std::string users_not_on_channels = s_users_not_on_channels.str();
    if (users_not_on_channels.length())
      command->reply(RPL_NAMREPLY, "*", users_not_on_channels);

    if (!channels.size() || users_not_on_channels.length())
      command->reply(RPL_ENDOFNAMES, "*");
  }
}
