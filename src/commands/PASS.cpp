#include "commands.hpp"

void PASS(Command *command) {

  User& user = command->get_user();
  std::string pass = command->get_arguments()[0];

  if (user.get_status() == ONLINE || user.get_status() == REGISTRATION) {
    command->reply(ERR_ALREADYREGISTRED);
  } else if (command->get_server().is_password_correct(pass)) {
    user.set_status(REGISTRATION);
  } else {
    command->reply(ERR_PASSWDMISMATCH);
  }
}
