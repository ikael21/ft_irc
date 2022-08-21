#include "commands.hpp"

void PONG(irc::Command* command) {
  command->get_user().set_state(irc::ACTIVE);
  command->get_user().set_last_activity(time(NULL));
}
