#include "errors.hpp"

/*
 * Error-ответы
 */

// 401
std::string ERR_NOSUCHNICK(std::string nick) { return nick + " :No such nick/channel"; }

// 402
std::string ERR_NOSUCHSERVER(std::string server) { return server + " :No such server"; }

// 403
std::string ERR_NOSUCHCHANNEL(std::string channel) { return channel + " :No such channel"; }

// 404
std::string ERR_CANNOTSENDTOCHAN(std::string channel) { return channel + " :Cannot send to channel"; }

// 405
std::string ERR_TOOMANYCHANNELS(std::string channel) { return channel + " :You have joined too many channels"; }

// 407
std::string ERR_TOOMANYTARGETS(std::string target) { return target + " :Duplicate recipients. No message delivered"; }

// 408
std::string ERR_NOORIGIN() { return ":No origin specified"; }

// 411
std::string ERR_NORECIPIENT(std::string command) { return ":No recipient given (" + command + ")"; }

// 412
std::string ERR_NOTEXTTOSEND() { return ":No text to send"; }

// 413
std::string ERR_NOTOPLEVEL(std::string mask) { return mask + " :No toplevel domain specified"; }

// 414
std::string ERR_WILDTOPLEVEL(std::string mask) { return mask + " :Wildcard in toplevel domain"; }

// 421
std::string ERR_UNKNOWNCOMMAND(std::string command) { return command + " :Unknown command"; }

// 424
std::string ERR_FILEERROR(std::string file, std::string file_op) { return ":File error doing " + file_op + " on " + file; }

// 431
std::string ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }

// 432
std::string ERR_ERRONEUSNICKNAME(std::string nick) { return nick + " :Erroneus nickname"; }

// 433
std::string ERR_NICKNAMEINUSE(std::string nick) { return nick + " :Nickname is already in use"; }

// 436
std::string ERR_NICKCOLLISION(std::string nick) { return nick + " :Nickname collision KILL"; }

// 442
std::string ERR_NOTONCHANNEL(std::string channel) { return channel + " :You're not on that channel"; }

// 443
std::string ERR_USERONCHANNEL(std::string user, std::string channel) { return user + " " + channel + " :is already on channel"; }

// 446
std::string ERR_USERSDISABLED() { return ":USERS has been disabled"; }

// 461
std::string ERR_NEEDMOREPARAMS(std::string command) { return command + " :Not enough parameters"; }

// 462
std::string ERR_ALREADYREGISTRED() { return ":You may not reregister"; }

// 464
std::string ERR_PASSWDMISMATCH() { return ":Password incorrect"; }

// 467
std::string ERR_KEYSET(std::string channel) { return channel + " :Channel key already set"; }

// 471
std::string ERR_CHANNELISFULL(std::string channel) { return channel + " :Cannot join channel (+l)"; }

// 472
std::string ERR_UNKNOWNMODE(std::string ch) { return ch + " :is unknown mode char to me"; }

// 473
std::string ERR_INVITEONLYCHAN(std::string channel) { return channel + " :Cannot join channel (+i)"; }

// 474
std::string ERR_BANNEDFROMCHAN(std::string channel) { return channel + " :Cannot join channel (+b)"; }

// 475
std::string ERR_BADCHANNELKEY(std::string channel) { return channel + " :Cannot join channel (+k)"; }

// 476
std::string ERR_BADCHANMASK() { return ":476"; }

// 482
std::string ERR_CHANOPRIVSNEEDED(std::string channel) { return channel + " :You're not channel operator"; }

// 491
std::string ERR_NOOPERHOST() { return ":No O-lines for your host"; }

// 501
std::string ERR_UMODEUNKNOWNFLAG() { return ":Unknown MODE flag"; }

// 502
std::string ERR_USERSDONTMATCH() { return ":Cant change mode for other users"; }

/*
 * Отклики команд
 */

// 301
std::string RPL_AWAY(std::string nick, std::string message) { return nick + " :" + message; }

// 305
std::string RPL_UNAWAY() { return ":You are no longer marked as being away"; }

// 306
std::string RPL_NOWAWAY() { return ":You have been marked as being away"; }

// 321
std::string RPL_LISTSTART() { return "Channel :Users  Name"; }

// 322
std::string RPL_LIST(std::string channel, std::string, std::string visible, std::string topic) {
   return channel + " " + visible + " :" + topic;
}

// 323
std::string RPL_LISTEND() { return ":End of /LIST"; }

// 324
std::string RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string mode_params) {
  return channel + " " + mode + " " + mode_params;
}

// 331
std::string RPL_NOTOPIC(std::string channel) { return channel + " :No topic is set"; }

// 332
std::string RPL_TOPIC(std::string channel, std::string topic) { return channel + " :" + topic; }

// 341
std::string RPL_INVITING(std::string channel, std::string nick) { return channel + " " + nick; }

/* 352
 * WHO nickname
 *:irc.example.org 352 go0h * aks E95F141D.8FD662D8.36133820.IP irc.example.org go0h H :0 aaa
 */
std::string RPL_WHOREPLY(
    std::string channel,
    std::string username,
    std::string hostname,
    std::string servername,
    std::string nick,
    std::string hopcount,
    std::string realname) {
  return (channel + " " + username + " " + hostname + " " + servername + " " + \
    nick + " <H|G>[*][@|+] :" + hopcount + " " + realname);
}

// 315
std::string RPL_ENDOFWHO(std::string name) { return name + " :End of /WHO list"; }

// 353
std::string RPL_NAMREPLY(std::string channel, std::string nick) { return channel + " :[@|+]" + nick; }

// 366
std::string RPL_ENDOFNAMES(std::string channel) { return channel + " :End of /NAMES list"; }

// 367
std::string RPL_BANLIST(std::string channel, std::string banid) { return channel + " " + banid; }

// 368
std::string RPL_ENDOFBANLIST(std::string channel) { return channel + " :End of channel ban list"; }

// 381
std::string RPL_YOUREOPER() { return ":You are now an IRC operator"; }

// 391
std::string RPL_TIME(std::string server, std::string string_time) {
  return  server + " :" + string_time;
}

// 392
std::string RPL_USERSSTART() { return ":UserID   Terminal  Host"; }

// 393
std::string RPL_USERS() { return ":%-8s %-9s %-8s"; }

// 394
std::string RPL_ENDOFUSERS() { return ":End of users"; }

// 395
std::string RPL_NOUSERS() { return ":Nobody logged in"; }

// 221
std::string RPL_UMODEIS(std::string user_mode) { return user_mode; }
