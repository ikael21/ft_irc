#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

#include "User.hpp"

User :: User(): _status(AUTHENTICATION) {};


User :: User(int fd) : _fd(fd), _status(AUTHENTICATION), _nick("*") {}


User :: User(int fd, std::string username, std::string hostname, std::string servername, std::string realname)
  : _fd(fd),
    _status(AUTHENTICATION),
    _username(username),
    _hostname(hostname),
    _servername(servername),
    _realname(realname),
    _nick("*") {}


User :: ~User() {}


bool operator==(const User& left, const User& right) {
  return left._username == right._username;
}


bool operator==(const User& left, const int fd) {
  return left._fd == fd;
}


void User :: receive(ssize_t size) {
  char* buff = new char[size + 1];

  size = recv(_fd, buff, size, 0);
  buff[size] = '\0';

  _buffer += buff;
  delete [] buff;
}


bool User :: hasNextMsg() {
  return _buffer.find(END_OF_MESSAGE) != std::string::npos;
}


std::string User :: getNextMsg() {

  size_t eol = _buffer.find(END_OF_MESSAGE);
  std::string line = _buffer.substr(0, eol);

  _buffer.erase(0, eol + strlen(END_OF_MESSAGE));
  return line;
}


void User :: sendMsgToUser(User& user, std::string message) {
  sendMsg(user.getFD(), message + END_OF_MESSAGE);
}


void User :: sendMsg(int fd, std::string message) {
  send(fd, message.c_str(), message.size(), 0);
}

/**
 * @brief Prefix of User's message that must be sent to other users/channels
 * :nick!username@hostname(?)
 */
std::string User :: getPrefixMessage() {
  return ":" + _nick + "!" + _username + "@" + _hostname;
}
