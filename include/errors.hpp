
#ifndef ERRORS_HPP_
# define ERRORS_HPP_

#include <string>

typedef enum  s_irc_error {

  /* Error-ответы */
  ERR_NOSUCHNICK        = 401,
  ERR_NOSUCHSERVER      = 402,
  ERR_NOSUCHCHANNEL     = 403,
  ERR_CANNOTSENDTOCHAN  = 404,
  ERR_TOOMANYCHANNELS   = 405,
  ERR_TOOMANYTARGETS    = 407,
  ERR_NOORIGIN          = 408,
  ERR_NORECIPIENT       = 411,
  ERR_NOTEXTTOSEND      = 412,
  ERR_NOTOPLEVEL        = 413,
  ERR_WILDTOPLEVEL      = 414,
  ERR_UNKNOWNCOMMAND    = 421,
  ERR_FILEERROR         = 424,
  ERR_NONICKNAMEGIVEN   = 431,
  ERR_ERRONEUSNICKNAME  = 432,
  ERR_NICKNAMEINUSE     = 433,
  ERR_NICKCOLLISION     = 436,
  ERR_USERNOTINCHANNEL  = 441,
  ERR_NOTONCHANNEL      = 442,
  ERR_USERONCHANNEL     = 443,
  ERR_USERSDISABLED     = 446,
  ERR_NOTREGISTERED     = 451,
  ERR_NEEDMOREPARAMS    = 461,
  ERR_ALREADYREGISTRED  = 462,
  ERR_PASSWDMISMATCH    = 464,
  ERR_KEYSET            = 467,
  ERR_CHANNELISFULL     = 471,
  ERR_UNKNOWNMODE       = 472,
  ERR_INVITEONLYCHAN    = 473,
  ERR_BANNEDFROMCHAN    = 474,
  ERR_BADCHANNELKEY     = 475,
  ERR_BADCHANMASK       = 476,
  ERR_CHANOPRIVSNEEDED  = 482,
  ERR_NOOPERHOST        = 491,
  ERR_UMODEUNKNOWNFLAG  = 501,
  ERR_USERSDONTMATCH    = 502,

  /* Отклики команд */
  RPL_AWAY              = 301,
  RPL_UNAWAY            = 305,
  RPL_NOWAWAY           = 306,
  RPL_LISTSTART         = 321,
  RPL_LIST              = 322,
  RPL_LISTEND           = 323,
  RPL_CHANNELMODEIS     = 324,
  RPL_NOTOPIC           = 331,
  RPL_TOPIC             = 332,
  RPL_INVITING          = 341,
  RPL_WHOREPLY          = 352,
  RPL_ENDOFWHO          = 315,
  RPL_NAMREPLY          = 353,
  RPL_ENDOFNAMES        = 366,
  RPL_BANLIST           = 367,
  RPL_ENDOFBANLIST      = 368,
  RPL_YOUREOPER         = 381,
  RPL_TIME              = 391,
  RPL_USERSSTART        = 392,
  RPL_USERS             = 393,
  RPL_ENDOFUSERS        = 394,
  RPL_NOUSERS           = 395,
  RPL_UMODEIS           = 221,
  RPL_MOTDSTART         = 375,
  RPL_MOTD              = 372,
  RPL_ENDOFMOTD         = 376
}             t_irc_error;

std::string irc_error(t_irc_error err_code,
                      std::string a1 = "",
                      std::string a2 = "",
                      std::string a3 = "",
                      std::string a4 = "",
                      std::string a5 = "",
                      std::string a6 = "",
                      std::string a7 = "");

# endif
