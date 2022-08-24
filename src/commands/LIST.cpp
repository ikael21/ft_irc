#include "commands.hpp"


void check_and_return_channels(irc::Command *command, irc::IrcServer::t_channel_list::iterator& ch) {

  if(!ch->is_private() || \
    ((ch->is_private() || ch->is_secret()) && ch->user_on_channel(command->get_user()))) {

    std::stringstream num_visible_users;
    num_visible_users << ch->get_visible_users().size();

    command->reply(RPL_LIST, ch->get_name(), num_visible_users.str(), ch->get_topic());
  }
}


void LIST(irc::Command* command) {

  std::vector<std::string> args = command->get_arguments();
  irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();

  command->reply(RPL_LISTSTART);

  if (args.size()) {
    std::vector<std::string> channel_list = split(args[0], ',');

    for (size_t i = 0; i < channel_list.size(); ++i) {
      irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channel_list[i]);
      if (ch != channels.end()) {
        check_and_return_channels(command, ch);
      }
    }
  }
  else {
    for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
      check_and_return_channels(command, ch);
    }
  }
  command->reply(RPL_LISTEND);
}
