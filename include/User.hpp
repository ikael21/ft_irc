#ifndef USER_HPP_
# define USER_HPP_

# define BUFFER_SIZE     1024
# define END_OF_MESSAGE  "\r\n"

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

  void set_fd(int fd) { _fd = fd; }
  int get_fd() const { return _fd; }

  void set_username(std::string username) { _username = username; }
  std::string get_username() { return _username; }

  void set_hostname(std::string hostname) { _hostname = hostname; }
  std::string get_hostname() { return _hostname; }

  void set_servername(std::string servername) { _servername = servername; }
  std::string get_servername() { return _servername; }

  void set_realname(std::string realname) { _realname = realname; }
  std::string get_realname() { return _realname; }

  void set_nick(std::string nick) { _nick = nick; }
  std::string get_nick() { return _nick; }

  void set_afk_msg(std::string message) { _afkMessage = message; }
  std::string get_afk_msg() { return _afkMessage; }

  void set_quit_msg(std::string message) { _quitMessage = message; }
  std::string get_quit_msg() { return _quitMessage; }

  void set_status(t_user_status status) { _status = status; }
  t_user_status get_status() { return _status; }

  friend bool operator==(const User& left, const User& right);
  friend bool operator!=(const User& left, const User& right);


  void receive(ssize_t size);
  bool has_msg();
  std::string get_next_msg();

  void send_msg(int fd, std::string message);
  void send_msg_to_user(User& user, std::string message);

  std::string get_prefix_msg();

  bool is_away() { return !_afkMessage.empty(); }

  const std::string& get_buffer() const { return _buffer; }

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
