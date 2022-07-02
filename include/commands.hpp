
#include "IrcServer.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Command.hpp"
#include "errors.hpp"


void PASS(Command *command);

void NICK(Command *command);

void USER(Command *command);

void PRIVMSG(Command *command);
