#include "Command.hpp"
#include "IrcServer.hpp"
#include "User.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "commands.hpp"


irc::Command::t_command_struct command_arr[] = {
  { "AWAY",    &AWAY,    0L },
  { "INVITE",  &INVITE,  2L },
  { "JOIN",    &JOIN,    1L },
  { "KICK",    &KICK,    2L },
  { "LIST",    &LIST,    0L },
  { "MODE",    &MODE,    1L },
  { "NAMES",   &NAMES,   0L },
  { "NICK",    &NICK,    0L },
  { "NOTICE",  &NOTICE,  0L },
  { "PART",    &PART,    1L },
  { "PASS",    &PASS,    1L },
  { "PONG",    &PONG,    1L },
  { "PRIVMSG", &PRIVMSG, 0L },
  { "QUIT",    &QUIT,    0L },
  { "TIME",    &TIME,    0L },
  { "TOPIC",   &TOPIC,   0L },
  { "USER",    &USER,    4L },
  { "WHOIS",   &WHOIS,   1L }
};


irc::Command::Command(irc::IrcServer &server, User& user, std::string command)
  : _server(server), _user(user), _full_message(command), _command_name("") {

  bzero(&_command, sizeof(t_command_struct));

  _arguments = split(command, ' ', 1);

  if (!_arguments.empty()) {

    _command_name = _arguments[0];
    std::transform(_command_name.begin(), _command_name.end(), _command_name.begin(), toupper);

    for (size_t i = 0; i < sizeof(command_arr) / sizeof(t_command_struct); ++i) {
      if (_command_name == command_arr[i].func_name) {
        _command = command_arr[i];
        break;
      }
    }
    _arguments.erase(_arguments.begin());

    if (_command.min_args && !_arguments.empty()) {
      _arguments = split(_arguments[1], ' ', _command.min_args + 1);
    }
  }
}


void irc::Command::execute() {

  // check if User Registered
  if (_user.get_status() != ONLINE) {
    if (_user.get_status() == AUTHENTICATION && _command_name != "PASS") {
      return reply(ERR_NOTREGISTERED);
    } else if (_user.get_status() == REGISTRATION &&  \
        (_command_name != "NICK" && _command_name != "USER")) {
      return reply(ERR_NOTREGISTERED);
    }
  }

  // basic checks
  if (_command_name.size() != 0L && _command.func == NULL) {
    return reply(ERR_UNKNOWNCOMMAND, _command_name);
  } else if (_arguments.size() == 0L && _command.func == NULL) {
    return;
  } else if (_command.min_args > _arguments.size()) {
    return reply(ERR_NEEDMOREPARAMS, _command.func_name);
  }

  _command.func(this);
}


void irc::Command::reply(t_irc_error err_code, std::string a1, std::string a2, std::string a3,
                      std::string a4, std::string a5, std::string a6, std::string a7) {

  std::stringstream msg;

  msg << ":" << _server.get_server_name() << " " << err_code << " "
    << _user.get_nick() << " " << irc_error(err_code, a1, a2, a3, a4, a5, a6, a7);

  // all messages send from User class, but I think that it must be sent something like that
  // _server.sendMsgToUser(_user, msg.str());
  _user.send_msg_to_user(_user, msg.str());

  // закоментил, т.к. если от клиента несколько команд приходит, он удаляет все кроме первой
  // _user.get_buffer().clear();
}
