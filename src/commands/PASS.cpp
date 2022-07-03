#include "commands.hpp"

void PASS(Command *command) {

  User &user = command->getUser();
  std::string pass = command->getArguments()[0];

  if (user.getStatus() == ONLINE || user.getStatus() == REGISTRATION) {
    command->reply(ERR_ALREADYREGISTRED);
  } else if (command->getServer().isCorrectPassword(pass)) {
    user.setStatus(REGISTRATION);
  } else {
    command->reply(ERR_PASSWDMISMATCH);
  }
}
