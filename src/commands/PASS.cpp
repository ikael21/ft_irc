#include "commands.hpp"

void PASS(Command *command) {

  User &user = command->getUser();
  std::string pass = command->getArguments()[0];

  if (user.getStatus() == ONLINE || user.getStatus() == REGISTRATION) {
    user.sendMsgToUser(user, irc_error(ERR_ALREADYREGISTRED));
  } else if (command->getServer().isCorrectPassword(pass)) {
    user.setStatus(REGISTRATION);
  } else {
    user.sendMsgToUser(user, irc_error(ERR_PASSWDMISMATCH));
  }
}
