#include <sstream>

#include "commands.hpp"
#include "utils.hpp"

void PRIVMSG(Command *command) {

  User& user = command->getUser();
  std::vector<std::string> args = command->getArguments();
  std::vector<std::string> recipients;
  std::string message;

  if (args.empty())
    return command->reply(ERR_NORECIPIENT, command->getCommandName());

  args = split(args[0], ' ', 1);
  recipients = split(args[0], ',');

  std::cout << args.size() << std::endl;
  if (recipients.empty())
    return command->reply(ERR_NORECIPIENT, command->getCommandName());

  if (args.size() < 2)
    return command->reply(ERR_NOTEXTTOSEND);

  message = args[1];

  if (message[0] == ':')
    message.erase(0, 1);

  std::cout << message << std::endl;

  if (!message.length())
    return command->reply(ERR_NOTEXTTOSEND);

  //TODO
  // 1. Send to all recipients
  // 2. Check if recipient Exists     - ERR_NOSUCHNICK
  // 3. Check if User on channel      - ERR_CANNOTSENDTOCHAN
  // 4. Check if recipient duplicates - ERR_TOOMANYTARGETS
  // 5. Check if User Away            - RPL_AWAY

  std::stringstream fullmessage;

  fullmessage << user.getPrefixMessage() << " " << \
    command->getCommandName() << " " << user.getNick() << " :" << message;

  std::cout << "Need to send message: '" + fullmessage.str() << std::endl;
}
