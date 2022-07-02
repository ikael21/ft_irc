#include <cstring>
#include "commands.hpp"

void NICK(Command *command) {
  std::string special("-_[]{}\\`|"); // https://dev.to/nichartley/how-do-you-use-irc-311f
  User &user = command->getUser();

  if (!command->numArgs())
    return user.sendMsgToUser(user, irc_error(ERR_NONICKNAMEGIVEN));

  std::string nick = command->getArguments()[0];

  if (!isalpha(nick[0]) && special.find(nick[0]) == std::string::npos)
    return user.sendMsgToUser(user, irc_error(ERR_ERRONEUSNICKNAME, nick));

  for (size_t i = 1; i < nick.length(); ++i) {
    if (!isalnum(nick[i]) && special.find(nick[i]) == std::string::npos)
      return user.sendMsgToUser(user, irc_error(ERR_ERRONEUSNICKNAME, nick));
  }

  // TODO
  // 1. Check if NICK exists on server
  // 2. Send to all channels that user changes his nickname

  // if User has Username and Nick his status - ONLINE
  if (user.getUsername().length()) {
    user.setStatus(ONLINE);
  }

  user.setNick(nick);
}
