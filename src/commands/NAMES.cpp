#include "commands.hpp"

// TODO NEED TEST
void check_and_return_names(Command *command, irc::IrcServer::t_channel_list::iterator& ch) {

  if(!ch->is_private() || \
    ((ch->is_private() || ch->is_secret()) && ch->user_on_channel(command->get_user()))) {

    std::vector<User*> channel_users = ch->get_users();
    std::stringstream users_str;

    for (size_t i = 0; i < channel_users.size(); ++i) {
      users_str << (ch->is_oper(*channel_users[i]) ? "@" : "") << channel_users[i]->get_nick() << " ";
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
    if (!channels.size())
      command->reply(RPL_ENDOFNAMES, "*");
    else {
      for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
        check_and_return_names(command, ch);
      }
    }

    //TODO список пользователей,
    //  которые видимые, но не находятся ни на одной канале, или не на одном
    //  видимом канале, которые начинаюся как 'channel' "*".
  }
}
