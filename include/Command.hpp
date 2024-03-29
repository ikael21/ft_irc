#ifndef COMMAND_HPP_
# define COMMAND_HPP_

# include <string>
# include <vector>
# include <map>

# include "IrcServer.hpp"
# include "User.hpp"
# include "errors.hpp"

namespace irc {

class Command {

 public:
  typedef void (*command_func)(Command *);

  typedef struct  s_command_struct {
    char          func_name[20];
    command_func  func;
    size_t        min_args;
  }               t_command_struct;

  Command(irc::IrcServer& server, User& user, std::string command);

  std::string& get_full_msg() { return _full_message; }
  std::string get_command_name() { return _command.func_name; }
  std::vector<std::string>& get_arguments() { return _arguments; }
  size_t num_args() { return _arguments.size(); }

  irc::IrcServer& get_server() { return _server; }
  User& get_user() { return _user; }

  void execute();

  void reply(t_irc_error err_code,
              std::string a1 = "",
              std::string a2 = "",
              std::string a3 = "",
              std::string a4 = "",
              std::string a5 = "",
              std::string a6 = "",
              std::string a7 = "");

 private:
  irc::IrcServer&           _server;
  User&                     _user;
  std::string               _full_message;
  std::string               _command_name;
  t_command_struct          _command;
  std::vector<std::string>  _arguments;
};

} // namespace irc

# endif
