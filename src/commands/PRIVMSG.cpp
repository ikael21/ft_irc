#include "commands.hpp"
#include "utils.hpp"

void PRIVMSG(Command *command) {

  User& user = command->get_user();
  std::vector<std::string>& args = command->get_arguments();
  std::vector<std::string> recipients;
  std::string message;

  if (args.empty())
    return command->reply(ERR_NORECIPIENT, command->get_command_name());

  args = split(args[0], ' ', 1);
  recipients = split(args[0], ',');

  if (recipients.empty())
    return command->reply(ERR_NORECIPIENT, command->get_command_name());

  if (args.size() < 2)
    return command->reply(ERR_NOTEXTTOSEND);

  message = args[1];

  if (message[0] == ':')
    message.erase(0, 1);

  if (!message.length())
    return command->reply(ERR_NOTEXTTOSEND);


  for (std::vector<std::string>::iterator recipient = recipients.begin();
        recipient != recipients.end(); ++recipient) {

    //check if target duplicates
    if (std::find(recipients.begin(), recipients.end(), *recipient) != recipient) {
      command->reply(ERR_TOOMANYTARGETS, *recipient);
      continue;
    }

    if (is_channel(*recipient)) {
      irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();
      irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), *recipient);

      if (ch == channels.end()) {
        command->reply(ERR_NOSUCHNICK, *recipient);
      } else {

        if (ch->is_banned(user)) {
          command->reply(ERR_CANNOTSENDTOCHAN, *recipient);
        } else if ((ch->have_mode(CH_FORBID_OUT_MSG) && ch->user_on_channel(user)) || !ch->have_mode(CH_FORBID_OUT_MSG)) {
          std::string fullmessage = user.get_prefix_msg() + \
            command->get_command_name() + " " + ch->get_name() + " :" + message;

          ch->send_to_channel(user, fullmessage);
        } else {
          command->reply(ERR_CANNOTSENDTOCHAN, *recipient);
        }
      }
    } else {

      try {
        User& to = command->get_server().get_user_by_nickname(*recipient);

        if (to.is_away())
          command->reply(RPL_AWAY, to.get_nick(), to.get_afk_msg());

        std::string fullmessage = user.get_prefix_msg() + \
          command->get_command_name() + " " + to.get_nick() + " :" + message;

        user.send_msg_to_user(to, fullmessage);
      } catch (UserNotFound& e) {
        command->reply(ERR_NOSUCHNICK, *recipient);
      }
    }
  }
}
