#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

#include "User.hpp"

User :: User(): _status(AUTHENTICATION) {};


User :: User(int fd) : _fd(fd), _status(AUTHENTICATION) {}


User :: User(int fd, std::string username, std::string hostname, std::string servername, std::string realname)
  : _fd(fd),
    _status(AUTHENTICATION),
    _username(username),
    _hostname(hostname),
    _servername(servername),
    _realname(realname) {}


User :: ~User() {}


bool operator==(const User& left, const User& right) {
  return left._username == right._username;
}


bool operator==(const User& left, const int fd) {
  return left._fd == fd;
}


std::string User :: receiveMsg(ssize_t size) {
  char    buff[size + 1];

  size = recv(_fd, buff, size, 0);

  if (size == -1)
    return "";
  buff[size] = '\0';

  _buffer += buff;

  size_t eof = _buffer.find(END_OF_MESSAGE);

  if (_buffer.find(END_OF_MESSAGE) == std::string::npos) {
    return "";
  }

  std::string message = _buffer.substr(0, eof);
  _buffer.erase(0, eof + strlen(END_OF_MESSAGE));

  return message;
}


void User :: sendMsgToUser(User& user, std::string message) {
  sendMsg(user.getFD(), message + END_OF_MESSAGE);
}


void User :: sendMsg(int fd, std::string message) {
  send(fd, message.c_str(), message.size(), 0);
}
