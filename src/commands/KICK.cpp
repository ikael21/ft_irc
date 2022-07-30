#include "commands.hpp"

/* KICK
 * Параметры: <channel> <user> [<comment>]
 */
void KICK(Command* command) {

  User& user = command->get_user();
  std::vector<std::string>& args = command->get_arguments();
  irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();
  irc::IrcServer::t_channel_list::iterator channel = std::find(channels.begin(), channels.end(), args[0]);

  if (channel == channels.end())
    return command->reply(ERR_NOSUCHCHANNEL, args[0]);
  else if (!channel->user_on_channel(user))
    return command->reply(ERR_NOTONCHANNEL, args[0]);
  else if (!channel->is_oper(user))
    return command->reply(ERR_CHANOPRIVSNEEDED, channel->get_name());

  try {
    User& kicked_user = command->get_server().get_user_by_nickname(args[1]);

    if (!channel->user_on_channel(kicked_user))
      return command->reply(ERR_USERNOTINCHANNEL, args[1], args[0]);

    std::string kick_msg = user.get_prefix_msg() + command->get_command_name() + " " \
      + channel->get_name() + " " + kicked_user.get_nick();

    if (args.size() == 3)
      kick_msg = kick_msg + " " + (args[2][0] == ':' ? "" : ":") + args[2];

    channel->send_to_channel(user, kick_msg);
    user.send_msg_to_user(user, kick_msg); // CHECK!
    channel->remove_user(kicked_user);
  } catch (UserNotFound& e) {
    command->reply(ERR_NOTONCHANNEL, args[0]);
  }
}