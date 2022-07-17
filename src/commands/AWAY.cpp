#include "commands.hpp"

void AWAY(Command *command) {

  User &user = command->get_user();
  std::vector<std::string>& args = command->get_arguments();
  std::string message;

  if (!args.size()) {
    user.set_afk_msg(std::string());
    return command->reply(RPL_UNAWAY);
  }

  message = args[0];
  if (message[0] == ':')
    message.erase(0, 1);

  user.set_afk_msg(message);

  if (message.empty())
    return command->reply(RPL_UNAWAY);

  return command->reply(RPL_NOWAWAY);
}
