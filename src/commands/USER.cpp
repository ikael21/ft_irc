#include "commands.hpp"

std::string logo[] = {
  "",
  "██████╗  ██╗    ██╗██████╗  ██████╗",
  "╚════██╗███║    ██║██╔══██╗██╔════╝",
  " █████╔╝╚██║    ██║██████╔╝██║     ",
  "██╔═══╝  ██║    ██║██╔══██╗██║     ",
  "███████╗ ██║    ██║██║  ██║╚██████╗",
  "╚══════╝ ╚═╝    ╚═╝╚═╝  ╚═╝ ╚═════╝",
  ""
};

void __print_logo(Command *command) {
  command->reply(RPL_MOTDSTART, "irc.21-school.ru");
  for (size_t i = 0; i < sizeof(logo) / sizeof(std::string); ++i) {
    command->reply(RPL_MOTD, logo[i]);
  }
  command->reply(RPL_ENDOFMOTD);
}

void USER(Command *command) {

  User &user = command->get_user();

  if (command->get_user().get_status() == ONLINE)
		return command->reply(ERR_ALREADYREGISTRED);

  user.set_username(command->get_arguments()[0]);
  user.set_realname(command->get_arguments()[3]);

  /* Servername and Hostname sets when user is connecting to Server
   * user.setServername(command->getArguments()[2]);
   * user.setHostname(command->getArguments()[1]);
   */

  // if User has Nick and Username - ONLINE
  if (user.get_nick() != "*") {
    user.set_status(ONLINE);
    __print_logo(command);
  }
}
