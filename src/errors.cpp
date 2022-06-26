#include "errors.hpp"

/*
 * Error-ответы
 */

// 401
std::string __ERR_NOSUCHNICK(std::string nick) { return nick + " :No such nick/channel"; }

// 402
std::string __ERR_NOSUCHSERVER(std::string server) { return server + " :No such server"; }

// 403
std::string __ERR_NOSUCHCHANNEL(std::string channel) { return channel + " :No such channel"; }

// 404
std::string __ERR_CANNOTSENDTOCHAN(std::string channel) { return channel + " :Cannot send to channel"; }

// 405
std::string __ERR_TOOMANYCHANNELS(std::string channel) { return channel + " :You have joined too many channels"; }

// 407
std::string __ERR_TOOMANYTARGETS(std::string target) { return target + " :Duplicate recipients. No message delivered"; }

// 408
std::string __ERR_NOORIGIN() { return ":No origin specified"; }

// 411
std::string __ERR_NORECIPIENT(std::string command) { return ":No recipient given (" + command + ")"; }

// 412
std::string __ERR_NOTEXTTOSEND() { return ":No text to send"; }

// 413
std::string __ERR_NOTOPLEVEL(std::string mask) { return mask + " :No toplevel domain specified"; }

// 414
std::string __ERR_WILDTOPLEVEL(std::string mask) { return mask + " :Wildcard in toplevel domain"; }

// 421
std::string __ERR_UNKNOWNCOMMAND(std::string command) { return command + " :Unknown command"; }

// 424
std::string __ERR_FILEERROR(std::string file, std::string file_op) { return ":File error doing " + file_op + " on " + file; }

// 431
std::string __ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }

// 432
std::string __ERR_ERRONEUSNICKNAME(std::string nick) { return nick + " :Erroneus nickname"; }

// 433
std::string __ERR_NICKNAMEINUSE(std::string nick) { return nick + " :Nickname is already in use"; }

// 436
std::string __ERR_NICKCOLLISION(std::string nick) { return nick + " :Nickname collision KILL"; }

// 442
std::string __ERR_NOTONCHANNEL(std::string channel) { return channel + " :You're not on that channel"; }

// 443
std::string __ERR_USERONCHANNEL(std::string user, std::string channel) { return user + " " + channel + " :is already on channel"; }

// 446
std::string __ERR_USERSDISABLED() { return ":USERS has been disabled"; }

// 451
std::string __ERR_NOTREGISTERED() { return ":You have not registered"; }

// 461
std::string __ERR_NEEDMOREPARAMS(std::string command) { return command + " :Not enough parameters"; }

// 462
std::string __ERR_ALREADYREGISTRED() { return ":You may not reregister"; }

// 464
std::string __ERR_PASSWDMISMATCH() { return ":Password incorrect"; }

// 467
std::string __ERR_KEYSET(std::string channel) { return channel + " :Channel key already set"; }

// 471
std::string __ERR_CHANNELISFULL(std::string channel) { return channel + " :Cannot join channel (+l)"; }

// 472
std::string __ERR_UNKNOWNMODE(std::string ch) { return ch + " :is unknown mode char to me"; }

// 473
std::string __ERR_INVITEONLYCHAN(std::string channel) { return channel + " :Cannot join channel (+i)"; }

// 474
std::string __ERR_BANNEDFROMCHAN(std::string channel) { return channel + " :Cannot join channel (+b)"; }

// 475
std::string __ERR_BADCHANNELKEY(std::string channel) { return channel + " :Cannot join channel (+k)"; }

// 476
std::string __ERR_BADCHANMASK() { return ":476"; }

// 482
std::string __ERR_CHANOPRIVSNEEDED(std::string channel) { return channel + " :You're not channel operator"; }

// 491
std::string __ERR_NOOPERHOST() { return ":No O-lines for your host"; }

// 501
std::string __ERR_UMODEUNKNOWNFLAG() { return ":Unknown MODE flag"; }

// 502
std::string __ERR_USERSDONTMATCH() { return ":Cant change mode for other users"; }

/*
 * Отклики команд
 */

// 301
std::string __RPL_AWAY(std::string nick, std::string message) { return nick + " :" + message; }

// 305
std::string __RPL_UNAWAY() { return ":You are no longer marked as being away"; }

// 306
std::string __RPL_NOWAWAY() { return ":You have been marked as being away"; }

// 321
std::string __RPL_LISTSTART() { return "Channel :Users  Name"; }

// 322
std::string __RPL_LIST(std::string channel, std::string visible, std::string topic) {
   return channel + " " + visible + " :" + topic;
}

// 323
std::string __RPL_LISTEND() { return ":End of /LIST"; }

// 324
std::string __RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string mode_params) {
  return channel + " " + mode + " " + mode_params;
}

// 331
std::string __RPL_NOTOPIC(std::string channel) { return channel + " :No topic is set"; }

// 332
std::string __RPL_TOPIC(std::string channel, std::string topic) { return channel + " :" + topic; }

// 341
std::string __RPL_INVITING(std::string channel, std::string nick) { return channel + " " + nick; }

/* 352
 * WHO nickname
 *:irc.example.org 352 go0h * aks E95F141D.8FD662D8.36133820.IP irc.example.org go0h H :0 aaa
 */
std::string __RPL_WHOREPLY(
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
std::string __RPL_ENDOFWHO(std::string name) { return name + " :End of /WHO list"; }

// 353
std::string __RPL_NAMREPLY(std::string channel, std::string nick) { return channel + " :[@|+]" + nick; }

// 366
std::string __RPL_ENDOFNAMES(std::string channel) { return channel + " :End of /NAMES list"; }

// 367
std::string __RPL_BANLIST(std::string channel, std::string banid) { return channel + " " + banid; }

// 368
std::string __RPL_ENDOFBANLIST(std::string channel) { return channel + " :End of channel ban list"; }

// 381
std::string __RPL_YOUREOPER() { return ":You are now an IRC operator"; }

// 391
std::string __RPL_TIME(std::string server, std::string string_time) {
  return  server + " :" + string_time;
}

// 392
std::string __RPL_USERSSTART() { return ":UserID   Terminal  Host"; }

// 393
std::string __RPL_USERS() { return ":%-8s %-9s %-8s"; }

// 394
std::string __RPL_ENDOFUSERS() { return ":End of users"; }

// 395
std::string __RPL_NOUSERS() { return ":Nobody logged in"; }

// 221
std::string __RPL_UMODEIS(std::string user_mode) { return user_mode; }

typedef struct  s_err {
  t_irc_error   err_code;
  void          *fptr;
  size_t        num_args;
}               t_err;

static t_err __error_arr[] = {
  { (t_irc_error)0, NULL, 0},
  { ERR_NOSUCHNICK, (void*)&__ERR_NOSUCHNICK, 1L },
  { ERR_NOSUCHSERVER, (void*)&__ERR_NOSUCHSERVER, 1L },
  { ERR_NOSUCHCHANNEL, (void*)&__ERR_NOSUCHCHANNEL, 1L },
  { ERR_CANNOTSENDTOCHAN, (void*)&__ERR_CANNOTSENDTOCHAN, 1L },
  { ERR_TOOMANYCHANNELS, (void*)&__ERR_TOOMANYCHANNELS, 1L },
  { ERR_TOOMANYTARGETS, (void*)&__ERR_TOOMANYTARGETS, 1L },
  { ERR_NOORIGIN, (void*)&__ERR_NOORIGIN, 0L },
  { ERR_NORECIPIENT, (void*)&__ERR_NORECIPIENT, 1L },
  { ERR_NOTEXTTOSEND, (void*)&__ERR_NOTEXTTOSEND, 0L },
  { ERR_NOTOPLEVEL, (void*)&__ERR_NOTOPLEVEL, 1L },
  { ERR_WILDTOPLEVEL, (void*)&__ERR_WILDTOPLEVEL, 1L },
  { ERR_UNKNOWNCOMMAND, (void*)&__ERR_UNKNOWNCOMMAND, 1L },
  { ERR_FILEERROR, (void*)&__ERR_FILEERROR, 2L },
  { ERR_NONICKNAMEGIVEN, (void*)&__ERR_NONICKNAMEGIVEN, 0L },
  { ERR_ERRONEUSNICKNAME, (void*)&__ERR_ERRONEUSNICKNAME, 1L },
  { ERR_NICKNAMEINUSE, (void*)&__ERR_NICKNAMEINUSE, 1L },
  { ERR_NICKCOLLISION, (void*)&__ERR_NICKCOLLISION, 1L },
  { ERR_NOTONCHANNEL, (void*)&__ERR_NOTONCHANNEL, 1L },
  { ERR_USERONCHANNEL, (void*)&__ERR_USERONCHANNEL, 2L },
  { ERR_USERSDISABLED, (void*)&__ERR_USERSDISABLED, 0L },
  { ERR_NOTREGISTERED, (void*)&__ERR_NOTREGISTERED, 0L },
  { ERR_NEEDMOREPARAMS, (void*)&__ERR_NEEDMOREPARAMS, 1L },
  { ERR_ALREADYREGISTRED, (void*)&__ERR_ALREADYREGISTRED, 0L },
  { ERR_PASSWDMISMATCH, (void*)&__ERR_PASSWDMISMATCH, 0L },
  { ERR_KEYSET, (void*)&__ERR_KEYSET, 1L },
  { ERR_CHANNELISFULL, (void*)&__ERR_CHANNELISFULL,1L },
  { ERR_UNKNOWNMODE, (void*)&__ERR_UNKNOWNMODE, 1L },
  { ERR_INVITEONLYCHAN, (void*)&__ERR_INVITEONLYCHAN, 1L },
  { ERR_BANNEDFROMCHAN, (void*)&__ERR_BANNEDFROMCHAN, 1L },
  { ERR_BADCHANNELKEY, (void*)&__ERR_BADCHANNELKEY,1L },
  { ERR_BADCHANMASK, (void*)&__ERR_BADCHANMASK, 0L },
  { ERR_CHANOPRIVSNEEDED, (void*)&__ERR_CHANOPRIVSNEEDED, 1L },
  { ERR_NOOPERHOST, (void*)&__ERR_NOOPERHOST,0L },
  { ERR_UMODEUNKNOWNFLAG, (void*)&__ERR_UMODEUNKNOWNFLAG, 0L },
  { ERR_USERSDONTMATCH, (void*)&__ERR_USERSDONTMATCH, 0L },
  { RPL_AWAY, (void*)&__RPL_AWAY, 2L },
  { RPL_UNAWAY, (void*)&__RPL_UNAWAY, 0L },
  { RPL_NOWAWAY,  (void*)&__RPL_NOWAWAY, 0L },
  { RPL_LISTSTART, (void*)&__RPL_LISTSTART, 0L },
  { RPL_LIST, (void*)&__RPL_LIST, 3L },
  { RPL_LISTEND, (void*)&__RPL_LISTEND, 0L },
  { RPL_CHANNELMODEIS, (void*)&__RPL_CHANNELMODEIS, 3L },
  { RPL_NOTOPIC, (void*)&__RPL_NOTOPIC, 1L },
  { RPL_TOPIC, (void*)&__RPL_TOPIC, 2L },
  { RPL_INVITING, (void*)&__RPL_INVITING, 2L },
  { RPL_WHOREPLY, (void*)&__RPL_WHOREPLY, 7L },
  { RPL_ENDOFWHO, (void*)&__RPL_ENDOFWHO, 1L },
  { RPL_NAMREPLY, (void*)&__RPL_NAMREPLY, 2L },
  { RPL_ENDOFNAMES, (void*)&__RPL_ENDOFNAMES, 1L },
  { RPL_BANLIST, (void*)&__RPL_BANLIST, 2L },
  { RPL_ENDOFBANLIST, (void*)&__RPL_ENDOFBANLIST, 1L },
  { RPL_YOUREOPER, (void*)&__RPL_YOUREOPER, 0L },
  { RPL_TIME, (void*)&__RPL_TIME, 2L },
  { RPL_USERSSTART, (void*)&__RPL_USERSSTART, 0L },
  { RPL_USERS, (void*)&__RPL_USERS, 0L },
  { RPL_ENDOFUSERS, (void*)&__RPL_ENDOFUSERS, 0L },
  { RPL_NOUSERS, (void*)&__RPL_NOUSERS, 0L },
  { RPL_UMODEIS, (void*)&__RPL_UMODEIS, 1L }
};

t_err __get_error(t_irc_error err_code) {

  for (size_t i = 1; i < sizeof(__error_arr) / sizeof(t_err); ++i) {
    if (err_code == __error_arr[i].err_code) {
      return __error_arr[i];
    }
  }
  return __error_arr[0];
}

typedef std::string(*__f0)();
typedef std::string(*__f1)(std::string);
typedef std::string(*__f2)(std::string,std::string);
typedef std::string(*__f3)(std::string,std::string,std::string);
typedef std::string(*__f4)(std::string,std::string,std::string,std::string);
typedef std::string(*__f5)(std::string,std::string,std::string,std::string,std::string);
typedef std::string(*__f6)(std::string,std::string,std::string,std::string,std::string,std::string);
typedef std::string(*__f7)(std::string,std::string,std::string,std::string,std::string,std::string,std::string);


std::string irc_error(t_irc_error err_code,
                      std::string a1,
                      std::string a2,
                      std::string a3,
                      std::string a4,
                      std::string a5,
                      std::string a6,
                      std::string a7) {

  t_err err_s = __get_error(err_code);
  if (err_s.fptr == NULL)
    return std::string();

  switch (err_s.num_args)
  {
    case 0L:
      return ((__f0)err_s.fptr)();
    case 1L:
      return ((__f1)err_s.fptr)(a1);
    case 2L:
      return ((__f2)err_s.fptr)(a1, a2);
    case 3L:
      return ((__f3)err_s.fptr)(a1, a2, a3);
    case 4L:
      return ((__f4)err_s.fptr)(a1, a2, a3, a4);
    case 5L:
      return ((__f5)err_s.fptr)(a1, a2, a3, a4, a5);
    case 6L:
      return ((__f6)err_s.fptr)(a1, a2, a3, a4, a5, a6);
    case 7L:
      return ((__f7)err_s.fptr)(a1, a2, a3, a4, a5, a6, a7);
  }
  return std::string();
}

