#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "utils.hpp"
#include "User.hpp"

User::User()
  : _status(AUTHENTICATION),
    _state(ACTIVE),
    _last_activity(time(NULL)) {}


User::User(int fd)
  : _fd(fd),
    _status(AUTHENTICATION),
    _nick("*"),
    _state(ACTIVE),
    _last_activity(time(NULL)) {}


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
    _last_activity(time(NULL)) {}


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
    _quitMessage(other._quitMessage),
    _buffer(other._buffer) {}


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


/**
 * state must be one of [ACTIVE, SEND_PING, WAIT_PONG]
**/
void User::set_state(int8_t state) { _state = state; }
