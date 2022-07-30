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


/* Список доступных режимов пользователя:
 *   i - делает пользователя невидимым;
 *   s - marks a user for receipt of server notices;
 *   w - user receives wallops;
 *   o - флаг оператора.
 */
typedef enum    s_user_mode {
  U_UNKNOWN_MODE = '\0',
  U_INVISIBLE    = 'i',
  U_OPERATOR     = 'o',
  U_WALLOPS      = 'w',
  U_S_NOTICE     = 's'
}               t_user_mode;


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
  const std::string& get_nick() const { return _nick; }

  void set_afk_msg(std::string message) { _afkMessage = message; }
  std::string get_afk_msg() { return _afkMessage; }
  bool is_away() { return !_afkMessage.empty(); }

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

  bool has_mode(t_user_mode mode);
  void add_mode(t_user_mode mode);
  void remove_mode(t_user_mode mode);
  bool is_invisible();

  std::string get_modes_as_str();

  const std::string& get_buffer() const { return _buffer; }

 private:
  int                      _fd;
  t_user_status            _status;
  std::string              _username;
  std::string              _hostname;
  std::string              _servername;
  std::string              _realname;
  std::string              _nick;
  std::string              _afkMessage;
  std::vector<t_user_mode> _modes;
  std::string              _buffer;
};

# endif
