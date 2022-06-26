#include "commands.hpp"

//TODO check NICK for correct symbols
void NICK(Command *command) {

  User &user = command->getUser();
  std::string nick = command->getArguments()[0];
  user.setNick(nick);
}
