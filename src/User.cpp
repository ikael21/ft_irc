
#include "User.hpp"

User :: User() {}

User :: User(int fd, std::string username, std::string hostname, std::string servername, std::string realname) :
  _fd(fd), _username(username), _hostname(hostname), _servername(servername), _realname(realname) {}

User :: ~User() {}

bool operator==(const User& left, const User& right) {
  return left._username == right._username;
}
