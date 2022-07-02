#ifndef COMMAND_HPP_
# define COMMAND_HPP_

# include <string>
# include <vector>
# include <map>

# include "IrcServer.hpp"
# include "User.hpp"

class Command {

 public:
  typedef void (*command_func)(Command *);

  typedef struct  s_command_struct {
    char          func_name[20];
    command_func  func;
    size_t        min_args;
  }               t_command_struct;

  Command(irc::IrcServer& server, User& user, std::string command);

  std::string getFullMessage() { return _full_message; }
  std::string getCommandName() { return _command.func_name; }
  std::vector<std::string> getArguments() { return _arguments; }
  size_t numArgs() { return _arguments.size(); }

  irc::IrcServer& getServer() { return _server; }
  User& getUser() { return _user; }

  void excecute();


 private:
  irc::IrcServer&           _server;
  User&                     _user;

  std::string               _full_message;
  std::string               _command_name;
  t_command_struct          _command;
  std::vector<std::string>  _arguments;
};

# endif
