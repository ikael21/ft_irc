
#include "IrcServer.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Command.hpp"
#include "errors.hpp"

void __print_logo(Command *command);

void PASS(Command *command);

void NICK(Command *command);

void USER(Command *command);

void PRIVMSG(Command *command);

void AWAY(Command *command);

void NOTICE(Command *command);

void JOIN(Command *command);

void PONG(Command *command);
