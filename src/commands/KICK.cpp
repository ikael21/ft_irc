#include "commands.hpp"

//TODO
// 1. Check if all users in channel show kick message
// 2. Check if user is not operator on Channel

void KICK(Command* command) {

    User& user = command->get_user();
    irc::IrcServer& server = command->get_server();
    std::vector<std::string> args = command->get_arguments();
    irc::IrcServer::t_channel_list channels = server.get_channels();


    std::string channel_name = args[0];
    std::string kick_msg;

    if (args.size() == 3)
      kick_msg = args[2];

    irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channel_name);

    if (ch == channels.end())
      return command->reply(ERR_NOSUCHCHANNEL, channel_name);
    else if (!ch->user_on_channel(user) || !ch->user_is_oper(user))
      return command->reply(ERR_CHANOPRIVSNEEDED, ch->get_name());

    try {
      User& kicked_user = server.get_user_by_nickname(args[1]);

      if (!ch->user_on_channel(kicked_user))
        return command->reply(ERR_NOTONCHANNEL, args[1]);

      ch->remove_user(kicked_user);
      if (!kick_msg.empty()) {
        kick_msg = user.get_prefix_msg() + command->get_command_name() + " " + kick_msg;
        ch->send_to_channel(user, kick_msg);
      }
    } catch (UserNotFound& e) {
      command->reply(ERR_NOTONCHANNEL, args[1]);
    }
}