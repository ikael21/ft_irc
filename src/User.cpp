#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "utils.hpp"
#include "User.hpp"

User::User()
  : _status(AUTHENTICATION),
    _state(ACTIVE),
    _last_activity(time(NULL)),
    _is_message_sent(true) {}


User::User(int fd)
  : _fd(fd),
    _status(AUTHENTICATION),
    _nick("*"),
    _state(ACTIVE),
    _last_activity(time(NULL)),
    _is_message_sent(true) {}


User::User(int fd, std::string username,
                   std::string hostname,
                   std::string servername,
                   std::string realname)
  : _fd(fd),
    _status(AUTHENTICATION),
    _username(username),
    _hostname(hostname),
    _servername(servername),
    _realname(realname),
    _nick("*"),
    _state(ACTIVE),
    _last_activity(time(NULL)),
    _is_message_sent(true) {}


User::User(const User& other)
  : _fd(other._fd),
    _status(other._status),
    _username(other._username),
    _hostname(other._hostname),
    _servername(other._servername),
    _realname(other._realname),
    _nick(other._nick),
    _state(other._state),
    _last_activity(other._last_activity),
    _afkMessage(other._afkMessage),
    _buffer(other._buffer),
    _is_message_sent(true) {}


User::~User() {}


bool operator==(const User& left, const User& right) {
  return left._fd == right._fd;
}


bool operator!=(const User& left, const User& right) {
    return left._fd != right._fd;
}


void User::receive(ssize_t size) {
  char* buff = new char[size + 1];

  size = recv(_fd, buff, size, 0);
  buff[size] = '\0';

  _buffer += buff;
  delete [] buff;
}


bool User::has_msg() {
  return _buffer.find(END_OF_MESSAGE) != std::string::npos;
}


std::string User::get_next_msg() {

  size_t eol = _buffer.find(END_OF_MESSAGE);
  std::string line = _buffer.substr(0, eol);

  _buffer.erase(0, eol + strlen(END_OF_MESSAGE));
  return line;
}


void User::send_msg_to_user(User& user, std::string message) {
  send_msg(user.get_fd(), message + END_OF_MESSAGE);
}


void User::send_msg(int fd, std::string message) {
  ssize_t bytes_sent = send(fd, message.c_str(), message.size(), 0);

  /*
  TODO check if all data sent

  ssize_t bytes_left = bytes_sent - static_cast<ssize_t>(message.size());
  if (bytes_left == 0) {
    _is_message_sent = true;
    return;
  }

  message.assign(message.begin() + bytes_sent, message.end());
  _is_message_sent = false;

  */

  #ifdef DEBUG
    std::cout << YELLOW "Reply for User(FD: "
      << fd << ")" RESET << std::endl;
    std::cout << GREEN "\t|" << message
      << RESET << std::endl;
  #endif
}

/**
 * @brief Prefix of User's message that must be sent to other users/channels
 * :nick!username@hostname(?)
 */
std::string User::get_prefix_msg() {
  return ":" + _nick + "!" + _username + "@" + _hostname + " ";
}


/**
 * state must be one of [ACTIVE, SEND_PING, WAIT_PONG]
**/
void User::set_state(int8_t state) { _state = state; }
bool User::has_mode(t_user_mode mode) {
  return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}


void User::add_mode(t_user_mode mode) {
  if (!has_mode(mode))
    _modes.push_back(mode);
}


void User::remove_mode(t_user_mode mode) {
  if (has_mode(mode))
    _modes.erase(std::find(_modes.begin(), _modes.end(), mode));
}


bool User::is_invisible() {
  return has_mode(U_INVISIBLE);
}


bool User::receive_notice() {
  return has_mode(U_S_NOTICE);
}


std::string User::get_modes_as_str() {
  std::string modes("+");
  for (size_t i = 0; i < _modes.size(); ++i)
    modes += static_cast<char>(_modes[i]);
  return modes;
}
