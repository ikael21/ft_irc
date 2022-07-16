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

  User &user = command->getUser();

  if (command->getUser().getStatus() == ONLINE)
		return command->reply(ERR_ALREADYREGISTRED);

  user.setUsername(command->getArguments()[0]);
  user.setHostname(command->getArguments()[1]);
  user.setServername(command->getArguments()[2]);
  user.setRealname(command->getArguments()[3]);

  // if User has Nick and Username - ONLINE
  if (user.getNick() != "*") {
    user.setStatus(ONLINE);
    __print_logo(command);
  }
}
