#include "commands.hpp"

std::string SPECIAL("-_[]{}\\`|"); // https://dev.to/nichartley/how-do-you-use-irc-311f


void NICK(irc::Command* command) {

  irc::User& user = command->get_user();

  if (!command->num_args())
    return command->reply(ERR_NONICKNAMEGIVEN);

  std::string nick = command->get_arguments()[0];

  if (!isalpha(nick[0]) && SPECIAL.find(nick[0]) == std::string::npos)
    return command->reply(ERR_ERRONEUSNICKNAME, nick);

  for (size_t i = 1; i < nick.length(); ++i) {
    if (!isalnum(nick[i]) && SPECIAL.find(nick[i]) == std::string::npos)
      return command->reply(ERR_ERRONEUSNICKNAME, nick);
  }

  try {
    irc::User& old_user = command->get_server().get_user_by_nickname(nick);

    if (user != old_user)
      command->reply(user.get_status() == irc::ONLINE ? ERR_NICKNAMEINUSE : ERR_NICKCOLLISION, nick);

  } catch (UserNotFound& e) {

    if (user.get_status() == irc::ONLINE) {
      /* Notify all users in channels where user is present */
      std::string msg = user.get_prefix_msg() + command->get_command_name() + " " + nick;
      irc::IrcServer::t_channel_list channels = command->get_server().get_channels();

      irc::IrcServer::t_userlist all_users = command->get_server().get_users();
      irc::IrcServer::t_userlist::iterator to = all_users.begin();

      while (to != all_users.end()) {
        for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
          if (ch->user_on_channel(user) && ch->user_on_channel(*to)) {
            user.send_msg_to_user(*to, msg);
            break;
          }
        }
        ++to;
      }
    }

    user.set_nick(nick);
    // if User has Username and Nick his status - ONLINE
    if (user.get_status() == irc::REGISTRATION && user.get_username().length()) {
      user.set_status(irc::ONLINE);
      __print_logo(command);
    }
  }
}
