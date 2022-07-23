#include "commands.hpp"


void PART(Command* command) {

  User& user = command->get_user();
  irc::IrcServer::t_channel_list& channels =  command->get_server().get_channels();

  std::string prefix_quit_msg = user.get_prefix_msg() + "PART ";
  std::vector<std::string> channels_names = split(command->get_arguments()[0], ',');

  for (size_t i = 0; i < channels_names.size(); ++i) {
    irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channels_names[i]);

    if (ch == channels.end()) {
      command->reply(ERR_NOSUCHCHANNEL, channels_names[i]);
    } else if (!ch->user_on_channel(user)) {
      command->reply(ERR_USERNOTINCHANNEL, user.get_nick(), ch->get_name());
    } else {
      std::string full_msg = prefix_quit_msg + ch->get_name();
      ch->send_to_channel(user, full_msg);
      ch->remove_user(user);

      if (ch->is_empty())
        channels.erase(ch);
    }
  }
}