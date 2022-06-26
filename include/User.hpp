
#ifndef USER_HPP_
# define USER_HPP_

# define BUFFER_SIZE     1024
# define END_OF_MESSAGE  "\n"

# include <string>
# include <vector>

typedef enum    s_user_status {
  AUTHENTICATION,
  REGISTRATION,
  ONLINE
}               t_user_status;

class User {

 public:
  User();
  User(int fd);
  User(int fd, std::string username, std::string hostname, std::string servername, std::string realname);
  ~User();

  void setFD(int fd) { _fd = fd; }
  int getFD() const { return _fd; }

  void setUsername(std::string username) { _username = username; }
  std::string getUsername() { return _username; }

  void setHostname(std::string hostname) { _hostname = hostname; }
  std::string getHostname() { return _hostname; }

  void setServername(std::string servername) { _servername = servername; }
  std::string getServername() { return _servername; }

  void setRealname(std::string realname) { _realname = realname; }
  std::string getRealname() { return _realname; }

  void setNick(std::string nick) { _nick = nick; }
  std::string getNick() { return _nick; }

  void setAFKMessage(std::string message) { _afkMessage = message; }
  std::string getAFKMessage() { return _afkMessage; }

  void setQuitMessage(std::string message) { _quitMessage = message; }
  std::string getQuitMessage() { return _quitMessage; }

  void setStatus(t_user_status status) { _status = status; }
  t_user_status getStatus() { return _status; }

  friend bool operator==(const User& left, const User& right);
  friend bool operator==(const User& left, const int fd);

  std::string receiveMsg();
  void sendMsg(int fd, std::string message);
  void sendMsgToUser(User& user, std::string message);

 private:
  int            _fd;
  t_user_status _status;
  std::string   _username;
  std::string   _hostname;
  std::string   _servername;
  std::string   _realname;
  std::string   _nick;

  std::string   _afkMessage;
  std::string   _quitMessage;

  std::string   _buffer;
};

# endif
