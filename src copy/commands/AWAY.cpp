#include "commands.hpp"

void AWAY(Command *command) {

  User &user = command->getUser();
  std::vector<std::string> args = command->getArguments();
  std::string message;

  if (!args.size())
    return command->reply(RPL_UNAWAY);

  message = args[0];
  if (message[0] == ':')
    message.erase(0, 1);

  if (message.empty())
    return command->reply(RPL_UNAWAY);
  else {
    user.setAFKMessage(message);
    return command->reply(RPL_NOWAWAY);
  }
}
