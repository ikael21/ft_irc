#include "commands.hpp"


void WHOIS(irc::Command* command) {

  std::string whois_nick = command->get_arguments()[0];

  try {
    irc::IrcServer& server = command->get_server();
    irc::User& who = server.get_user_by_nickname(whois_nick);
    irc::IrcServer::t_channel_list& channels = server.get_channels();

    command->reply(RPL_WHOISUSER, who.get_nick(), who.get_username(), who.get_hostname(), who.get_realname());
    std::string user_channels;

    for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
      if (ch->user_on_channel(who)) {
        user_channels += (ch->is_oper(who) ? "@" + ch->get_name() : ch->get_name()) + " ";
      }
    }
    command->reply(RPL_WHOISCHANNELS, who.get_nick(), user_channels);
    command->reply(RPL_WHOISSERVER, who.get_nick(), server.get_server_name(), "FT_IRC server based on TCP/IP protocol to RFC1459 standart");
  } catch (UserNotFound& e) {
    command->reply(ERR_NOSUCHNICK, whois_nick);
  }
  command->reply(RPL_ENDOFWHO, whois_nick);
}
