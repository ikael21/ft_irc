#include "commands.hpp"
#include "utils.hpp"

void PRIVMSG(Command *command) {

  User& user = command->getUser();
  std::vector<std::string> recipients;
  std::string cmd = command->getFullMessage();
  size_t i = command->getCommandName().length();

  for ( ; i < cmd.length(); ++i) {
    if (cmd[i] != ' ')
      break;
  }
  if (i == cmd.length())
    return user.sendMsgToUser(user, irc_error(ERR_NORECIPIENT, command->getCommandName()));

  cmd = cmd.erase(0, i);
  i = cmd.find(' ');
  if (i == std::string::npos)
    return user.sendMsgToUser(user, irc_error(ERR_NOTEXTTOSEND));

  recipients = split(cmd.substr(0, i), ',');
  cmd = cmd.erase(0, i);

  for (i = 0; i < cmd.length(); ++i) {
    if (cmd[i] != ' ')
      break;
  }

  if (i == cmd.length())
    return user.sendMsgToUser(user, irc_error(ERR_NOTEXTTOSEND));

  cmd = cmd.substr(i);
  if (cmd[0] == ':')
    cmd.erase(0, 1);

  if (!cmd.length())
    return user.sendMsgToUser(user, irc_error(ERR_NOTEXTTOSEND));

  //TODO
  // 1. Send to all recipients

  std::string message = command->getCommandName() + " " + user.getNick() + " :" + cmd;

  std::cout << "Need to send message: '" + message;
}
