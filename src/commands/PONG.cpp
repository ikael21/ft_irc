#include "commands.hpp"

void PONG(Command* command) {
  command->get_user().set_state(ACTIVE);
  command->get_user().set_last_activity(time(NULL));
}
