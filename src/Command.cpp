#include "Command.hpp"
#include "IrcServer.hpp"
#include "User.hpp"
#include "utils.h"
#include "errors.hpp"
#include "commands.hpp"


Command :: t_command_struct command_arr[] = {
  { "PASS", &PASS, 1L },
  { "NICK", &NICK, 1L }
};


Command :: Command(irc::IrcServer &server, User& user, std::string command)
  : _server(server), _user(user), _command_name("") {

  bzero(&_command, sizeof(t_command_struct));

  std::replace(command.begin(), command.end(), '\t', ' ');
  _arguments = split(command, ' ');

  if (_arguments.size()) {
    _command_name = _arguments[0];
    std::transform(_command_name.begin(), _command_name.end(), _command_name.begin(), toupper);

    for (size_t i = 0; i < sizeof(command_arr) / sizeof(t_command_struct); ++i) {
      if (_command_name == command_arr[i].func_name) {
        _command = command_arr[i];
        break;
      }
    }

    _arguments.erase(_arguments.begin());
  }
}



void Command :: excecute() {

  // basic checks
  if (_command_name.size() != 0L && _command.func == NULL) {

    // all messages send from User class, but I think that it must be sent
    // from IrcServer class because in start of message should be info about the server

    return _user.sendMsgToUser(_user, irc_error(ERR_UNKNOWNCOMMAND, _command_name));
  } else if (_arguments.size() == 0L && _command.func == NULL) {
    return;
  } else if (_command.min_args > _arguments.size()) {
    return _user.sendMsgToUser(_user, irc_error(ERR_NEEDMOREPARAMS, _command.func_name));
  }

  if (_user.getStatus() == AUTHENTICATION && _command_name != "PASS") {
    return _user.sendMsgToUser(_user, irc_error(ERR_NOTREGISTERED));
  } else if (_user.getStatus() == REGISTRATION &&  \
      (_command_name != "NICK" || _command_name != "USER")) {
    return _user.sendMsgToUser(_user, irc_error(ERR_NOTREGISTERED));
  }

  _command.func(this);
}
