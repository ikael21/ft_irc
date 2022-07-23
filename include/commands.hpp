
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

void LIST(Command *command);

void NAMES(Command *command);

void INVITE(Command *command);

void TOPIC(Command *command);

void KICK(Command *command);

void TIME(Command *command);

void USERS(Command *command);

void JOIN(Command *command);
