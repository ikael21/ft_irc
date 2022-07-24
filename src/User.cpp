#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "utils.hpp"
#include "User.hpp"

User::User(): _status(AUTHENTICATION) {};


User::User(int fd) : _fd(fd), _status(AUTHENTICATION), _nick("*") {}


User::User(int fd, std::string username, std::string hostname, std::string servername, std::string realname)
  : _fd(fd),
    _status(AUTHENTICATION),
    _username(username),
    _hostname(hostname),
    _servername(servername),
    _realname(realname),
    _nick("*") {}


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
  (void)bytes_sent;

#ifdef DEBUG
  std::cout << YELLOW "Reply for User(FD: "
    << fd << ")" RESET << std::endl;
  std::cout << GREEN "\t|" << message
    << RESET << std::endl;
#endif

  /*
    TODO check if all data sent

  size_t left_bytes =
    (!bytes_sent || bytes_sent == -1) ?
      0 : message.size() - static_cast<size_t>(bytes_sent);

  */
}

/**
 * @brief Prefix of User's message that must be sent to other users/channels
 * :nick!username@hostname(?)
 */
std::string User::get_prefix_msg() {
  return ":" + _nick + "!" + _username + "@" + _hostname + " ";
}


bool User::_has_mode(t_user_mode mode) {
  return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}


void User::add_mode(t_user_mode mode) {
  if (!_has_mode(mode))
    _modes.push_back(mode);
}


void User::remove_mode(t_user_mode mode) {
  if (_has_mode(mode))
    _modes.erase(std::find(_modes.begin(), _modes.end(), mode));
}


bool User::is_invisible() {
  return _has_mode(U_INVISIBLE);
}

std::string User::get_modes_as_str() {
  std::string modes("+");
  for (size_t i = 0; i < _modes.size(); ++i)
    modes += static_cast<char>(_modes[i]);
  return modes;
}