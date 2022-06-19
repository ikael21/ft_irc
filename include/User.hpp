
#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>

class User {

 public:
  User();
  User(int fd, std::string username, std::string hostname, std::string servername, std::string realname);
  ~User();

  void setFD(int fd) { _fd = fd; }
  int getFD() { return _fd; }

  void setUsername(int username) { _username = username; }
  std::string getUsername() { return _username; }

  void setHostname(int hostname) { _hostname = hostname; }
  std::string getHostname() { return _hostname; }

  void setServername(int servername) { _servername = servername; }
  std::string getServername() { return _servername; }

  void setRealname(int realname) { _realname = realname; }
  std::string getRealname() { return _realname; }

  void setAFKMessage(int message) { _afkMessage = message; }
  std::string getAFKMessage() { return _afkMessage; }

  void setQuitMessage(int message) { _quitMessage = message; }
  std::string getQuitMessage() { return _quitMessage; }

  friend bool operator==(const User& left, const User& right);

 private:
  int         _fd;
  std::string _username;
  std::string _hostname;
  std::string _servername;
  std::string _realname;

  std::string _afkMessage;
  std::string _quitMessage;
};

# endif
