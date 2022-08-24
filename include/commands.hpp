#include "IrcServer.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Command.hpp"
#include "errors.hpp"

void __print_logo(irc::Command* command);

void PASS(irc::Command* command);

void NICK(irc::Command* command);

void USER(irc::Command* command);

void PRIVMSG(irc::Command* command);

void AWAY(irc::Command* command);

void NOTICE(irc::Command* command);

void LIST(irc::Command* command);

void NAMES(irc::Command* command);

void INVITE(irc::Command* command);

void TOPIC(irc::Command* command);

void KICK(irc::Command* command);

void TIME(irc::Command* command);

void JOIN(irc::Command* command);

void PONG(irc::Command* command);

void QUIT(irc::Command* command);

void KICK(irc::Command* command);

void PART(irc::Command* command);

void MODE(irc::Command* command);

void assign_new_operator(irc::Command* command,
                         irc::User& user,
                         irc::IrcServer::t_channel_list::iterator ch);
