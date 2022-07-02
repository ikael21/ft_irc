#include "commands.hpp"

void USER(Command *command) {

  User &user = command->getUser();

  if (command->getUser().getStatus() == ONLINE)
		return user.sendMsgToUser(user, irc_error(ERR_ALREADYREGISTRED));

  user.setUsername(command->getArguments()[0]);
  user.setHostname(command->getArguments()[1]);
  user.setServername(command->getArguments()[2]);
  user.setServername(command->getArguments()[3]);

  // if User has Nick and Username - ONLINE
  if (user.getNick().length()) {
    user.setStatus(ONLINE);
  }
}
