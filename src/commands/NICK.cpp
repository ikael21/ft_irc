#include "commands.hpp"


void NICK(Command *command) {
  std::string special("-_[]{}\\`|"); // https://dev.to/nichartley/how-do-you-use-irc-311f
  User& user = command->get_user();
  irc::IrcServer& server = command->get_server();

  if (!command->num_args())
    return command->reply(ERR_NONICKNAMEGIVEN);

  std::string nick = command->get_arguments()[0];

  if (!isalpha(nick[0]) && special.find(nick[0]) == std::string::npos)
    return command->reply(ERR_ERRONEUSNICKNAME, nick);

  for (size_t i = 1; i < nick.length(); ++i) {
    if (!isalnum(nick[i]) && special.find(nick[i]) == std::string::npos)
      return command->reply(ERR_ERRONEUSNICKNAME, nick);
  }

  try {
    User& old_user = server.get_user_by_nickname(nick);

    if (user != old_user)
      command->reply(user.get_status() == ONLINE ? ERR_NICKNAMEINUSE : ERR_NICKCOLLISION, nick);

  } catch (UserNotFound &e) {

    if (user.get_status() == ONLINE) {
      /* Notify all users in channels where user is present */
      std::string msg = user.get_prefix_msg() + "NICK " + nick;
      irc::IrcServer::t_channel_list channels = server.get_channels();

      for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
        if ((*ch).user_on_channel(user))
          (*ch).send_to_channel(user, msg);
      }

      msg = ":" + user.get_nick() + " NICK " + nick;
      user.send_msg_to_user(user, msg);
    }

    user.set_nick(nick);
    // if User has Username and Nick his status - ONLINE
    if (user.get_status() == REGISTRATION && user.get_username().length()) {
      user.set_status(ONLINE);
      __print_logo(command);
    }
  }
}
