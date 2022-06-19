
#ifndef ERRORS_HPP
# define ERRORS_HPP

#include <string>

// 401
std::string ERR_NOSUCHNICK(std::string nick);

// 402
std::string ERR_NOSUCHSERVER(std::string server);

// 403
std::string ERR_NOSUCHCHANNEL(std::string channel);

// 404
std::string ERR_CANNOTSENDTOCHAN(std::string channel);

// 405
std::string ERR_TOOMANYCHANNELS(std::string channel);

// 407
std::string ERR_TOOMANYTARGETS(std::string target);

// 408
std::string ERR_NOORIGIN();

// 411
std::string ERR_NORECIPIENT(std::string command);

// 412
std::string ERR_NOTEXTTOSEND();

// 413
std::string ERR_NOTOPLEVEL(std::string mask);

// 414
std::string ERR_WILDTOPLEVEL(std::string mask);

// 421
std::string ERR_UNKNOWNCOMMAND(std::string command);

// 424
std::string ERR_FILEERROR(std::string file, std::string file_op);

// 431
std::string ERR_NONICKNAMEGIVEN();

// 432
std::string ERR_ERRONEUSNICKNAME(std::string nick);

// 433
std::string ERR_NICKNAMEINUSE(std::string nick);

// 436
std::string ERR_NICKCOLLISION(std::string nick);

// 442
std::string ERR_NOTONCHANNEL(std::string channel);

// 443
std::string ERR_USERONCHANNEL(std::string user, std::string channel);

// 446
std::string ERR_USERSDISABLED();

// 461
std::string ERR_NEEDMOREPARAMS(std::string command);

// 462
std::string ERR_ALREADYREGISTRED();

// 464
std::string ERR_PASSWDMISMATCH();

// 467
std::string ERR_KEYSET(std::string channel);

// 471
std::string ERR_CHANNELISFULL(std::string channel);

// 472
std::string ERR_UNKNOWNMODE(std::string ch);

// 473
std::string ERR_INVITEONLYCHAN(std::string channel);

// 474
std::string ERR_BANNEDFROMCHAN(std::string channel);

// 475
std::string ERR_BADCHANNELKEY(std::string channel);

// 476
std::string ERR_BADCHANMASK();

// 482
std::string ERR_CHANOPRIVSNEEDED(std::string channel);

// 491
std::string ERR_NOOPERHOST();

// 501
std::string ERR_UMODEUNKNOWNFLAG();

// 502
std::string ERR_USERSDONTMATCH();

/*
 * Отклики команд
 */

// 301
std::string RPL_AWAY(std::string nick, std::string message);

// 305
std::string RPL_UNAWAY();

// 306
std::string RPL_NOWAWAY();

// 321
std::string RPL_LISTSTART();

// 322
std::string RPL_LIST(std::string channel, std::string, std::string visible, std::string topic);

// 323
std::string RPL_LISTEND();
// 324
std::string RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string mode_params);
// 331
std::string RPL_NOTOPIC(std::string channel);

// 332
std::string RPL_TOPIC(std::string channel, std::string topic);

// 341
std::string RPL_INVITING(std::string channel, std::string nick);

// 352
std::string RPL_WHOREPLY(
    std::string channel,
    std::string user,
    std::string host,
    std::string server,
    std::string nick,
    std::string hopcount,
    std::string realname);

// 315
std::string RPL_ENDOFWHO(std::string name);

// 353
std::string RPL_NAMREPLY(std::string channel, std::string nick);

// 366
std::string RPL_ENDOFNAMES(std::string channel);

// 367
std::string RPL_BANLIST(std::string channel, std::string banid);

// 368
std::string RPL_ENDOFBANLIST(std::string channel);

// 381
std::string RPL_YOUREOPER();

// 391
std::string RPL_TIME(std::string server, std::string string_time);

// 392
std::string RPL_USERSSTART();

// 393
std::string RPL_USERS();

// 394
std::string RPL_ENDOFUSERS();

// 395
std::string RPL_NOUSERS();

// 221
std::string RPL_UMODEIS(std::string user_mode);

# endif
