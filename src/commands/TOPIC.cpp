#include "commands.hpp"


void TOPIC(Command *command) {

  User& user = command->get_user();
  irc::IrcServer& server = command->get_server();

  if (!command->num_args())
    return command->reply(ERR_NEEDMOREPARAMS, command->get_command_name());

  std::vector<std::string> args = split(command->get_arguments()[0], ' ', 1);

  std::string& channel_name = args[0];
  irc::IrcServer::t_channel_list& channels = server.get_channels();
  irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channel_name);

  if (ch == channels.end())
    return command->reply(ERR_NOSUCHCHANNEL, channel_name);

  if (args.size() == 1) {
    if (ch->get_topic().empty())
      return command->reply(RPL_NOTOPIC, channel_name);
    command->reply(RPL_TOPIC, channel_name, ch->get_topic());
  } else if (!ch->is_oper(user)) {
    command->reply(ERR_CHANOPRIVSNEEDED, channel_name);
  } else {

    std::string new_topic = args[1];
    if (new_topic[0] == ':')
      new_topic.erase(0, 1);

    ch->set_topic(new_topic);
    std::string notice = user.get_prefix_msg() + command->get_command_name() \
      + " " + channel_name + " :" + new_topic;

    ch->send_to_channel(user, notice);
    user.send_msg_to_user(user, notice);
  }
}
