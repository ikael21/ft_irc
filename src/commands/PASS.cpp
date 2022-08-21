#include "commands.hpp"

void PASS(irc::Command* command) {

  irc::User& user = command->get_user();
  std::string pass = command->get_arguments()[0];

  if (pass[0] == ':')
    pass.erase(0, 1);

  if (user.get_status() == irc::ONLINE || user.get_status() == irc::REGISTRATION) {
    command->reply(ERR_ALREADYREGISTRED);
  } else if (command->get_server().is_password_correct(pass)) {
    user.set_status(irc::REGISTRATION);
  } else {
    command->reply(ERR_PASSWDMISMATCH);
  }
}
