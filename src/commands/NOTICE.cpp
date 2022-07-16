#include <cstring>
#include "commands.hpp"

void NOTICE(Command *command) {

  User& user = command->get_user();
  std::vector<std::string> args = command->get_arguments();
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

  // TODO
  // 1. Send to all recipients
  // 2. Check if recipient Exists
  // 3. Check if User on channel
  // 4. Check if recipient duplicates
  // 5. Check if User Away

  std::stringstream fullmessage;

  fullmessage << user.get_prefix_msg() << " " << \
    command->get_command_name() << " " << user.get_nick() << " :" << message;

#ifdef DEBUG
  std::cout << "Need to send message: '" + fullmessage.str() + " to:" << std::endl;
#endif

  for (size_t i = 0; i < recipients.size(); ++i) {
    std::cout << recipients[i] << std::endl;
  }

}
