#include "commands.hpp"


void INVITE(Command *command) {

  User& user = command->get_user();
  std::vector<std::string> args = command->get_arguments();

  std::string& nick = args[0];
  std::string& channel_name = args[1];

  try {
    User& invited_user = command->get_server().get_user_by_nickname(nick);

    irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();
    irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channel_name);

    if (ch == channels.end())
      return command->reply(ERR_NOSUCHNICK, channel_name);
    else if (!ch->user_on_channel(user))
      return command->reply(ERR_NOTONCHANNEL, channel_name);
    else if (!ch->is_oper(user))
      return command->reply(ERR_CHANOPRIVSNEEDED, channel_name);
    else if (ch->user_on_channel(invited_user))
      return command->reply(ERR_USERONCHANNEL, nick, channel_name);

    ch->add_to_invite_list(invited_user);
    command->reply(RPL_INVITING, channel_name, nick);

    std::stringstream notice;
    notice << ":" << command->get_server().get_server_name()
      << " NOTICE @" << channel_name << " :" << user.get_nick()
      << " invited " << nick << " into the channel.";
    std::string msg = notice.str();

    std::vector<User*> channel_users = ch->get_users();
    for (size_t i = 0; i < channel_users.size(); ++i) {
      User& to = *channel_users[i];
      if (to.receive_notice())
        user.send_msg_to_user(to, msg);
    }

    if (invited_user.is_away()) {
      command->reply(RPL_AWAY, invited_user.get_nick(), invited_user.get_afk_msg());
    } else {
      std::string invite_msg = user.get_prefix_msg() + command->get_command_name() \
        + " " + nick + " :" + channel_name;
      user.send_msg_to_user(invited_user, invite_msg);
    }

  } catch (UserNotFound& e) {
    command->reply(ERR_NOSUCHNICK, nick);
  }
}
