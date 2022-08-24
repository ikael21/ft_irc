#include "commands.hpp"


void TIME(irc::Command* command) {
  std::vector<std::string>& args = command->get_arguments();
  if (args.size() > 0 && args[0] != command->get_server().get_server_name())
    return command->reply(ERR_NOSUCHSERVER);

  const time_t t = time(NULL);
  std::string tmp(ctime(&t));
  std::string current_time(tmp.substr(0, tmp.size() - 1));
  command->reply(RPL_TIME, current_time);
}
