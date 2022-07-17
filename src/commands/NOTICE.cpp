#include "commands.hpp"

void NOTICE(Command *command) {

  User& user = command->get_user();
  irc::IrcServer& server = command->get_server();
  std::vector<std::string>& args = command->get_arguments();
  std::vector<std::string> recipients;
  std::string message;

  if (args.empty())
    return;

  args = split(args[0], ' ', 1);
  recipients = split(args[0], ',');

  if (recipients.empty())
    return;

  sort(recipients.begin(), recipients.end());
  recipients.erase(unique(recipients.begin(), recipients.end() ), recipients.end());

  //  Check for Too many targets. The maximum is 1 for NOTICE.
  if (recipients.size() > 1)
    return;

  if (args.size() < 2)
    return;

  message = args[1];

  if (message[0] == ':')
    message.erase(0, 1);

  if (!message.length())
    return;

  if (is_channel(recipients[0])) {
    irc::IrcServer::t_channel_list& channels = server.get_channels();
    irc::IrcServer::t_channel_list::iterator it = std::find(channels.begin(), channels.end(), recipients[0]);

    if (it != channels.end()) {
      // TODO need more checks, with privileges on channel
      if (it->user_on_channel(user)) {
        std::string fullmessage = user.get_prefix_msg() + \
            command->get_command_name() + " " + it->get_name() + " :" + message;

        it->send_to_channel(user, fullmessage);
      }

    }
  } else {

    try {

      User& to = server.get_user_by_nickname(recipients[0]);

      std::string fullmessage = user.get_prefix_msg() + \
        command->get_command_name() + " " + user.get_nick() + " :" + message;

      user.send_msg_to_user(to, fullmessage);

    } catch (UserNotFound& e) { }
  }
}
