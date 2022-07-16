#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <stdexcept>
# include <iostream>

# include "User.hpp"

# ifndef DEBUG
  # define DEBUG 0
# endif

/* Список доступных режимов канала:
 *   o - брать/давать привилегии операторов канала
 *   p - флаг приватности канала;
 *   s - флаг секретности канала;
 *   i - флаг канала invite-only;
 *   t - при установке этого флага, менять топик могут только операторы;
 * ? n - запрещает сообщения на канал от посторонних клиентов;
 * ? m - модерируемый канал;
 *   l - установка ограничения на количество пользователей;
 * ? b - установка маски бана;
 * ? v - брать/давать возможность голоса при модерируемом режиме;
 *   k - установка на канал ключа (пароля).
 */

typedef enum    s_channel_mode {
  CH_CHANGE_PRIVILEGE   = 'o',
  CH_PRIVATE            = 'p',
  CH_SECRET             = 's',
  CH_INVITE_ONLY        = 'i',
  CH_TOPIC_MODIFIERS    = 't',
  CH_CHANGE_LIMIT_USERS = 'l',
  CH_CHANGE_KEY         = 'k'
}               t_channel_mode;

# define FORBIDDEN_CHANNEL_NAME_SYMBOLS "\t\n "
# define DEFAULT_USERS                  42
# define DEFAULT_CHANNEL_MODES          CH_TOPIC_MODIFIERS


/* Список доступных режимов юзера:
 *   i - делает пользователя невидимым;
 * ? s - marks a user for receipt of server notices;
 * ? w - user receives wallops;
 *   o - флаг оператора.
 */

typedef enum    s_user_mode {
  U_INVISIBLE   = 'i',
  U_OPERATOR    = 'o'
}               t_user_mode;

class Channel
{
 public:
  Channel();
  // Channel(std::string channelName, std::string username);

  ~Channel();

  bool is_private();

  void add_user(User& user);
  void remove_user(User& user);

  std::vector<User *> get_users() { return _users; }
  std::vector<User *> get_visible_users();

  void set_name(std::string channelName);
  std::string get_name() { return _name; }

  void set_topic(std::string topic);
  std::string get_topic() { return _topic; }

  void add_channel_mode(const std::string& mode);
  void remove_channel_mode(const std::string& mode);

  std::vector<t_channel_mode> get_channel_modes() { return _modes; }
  bool have_mode(t_channel_mode mode);

  void add_to_invite_list(User& user);
  bool is_invited(User& user);

  void add_to_ban_list(User& user);
  bool is_banned(User& user);

  void set_key(std::string key);
  bool has_key() { return _key.size(); }
  bool is_correct_key(std::string key) { return _key == key; }

  void set_limit_users(int limit);
  int get_limit_users() { return _limit_users; }
  bool is_full() { return _users.size() >= _limit_users; }

  void add_mode_to_user(User& user, const std::string& mode);
  void add_mode_to_user(User& user, t_user_mode mode);

  void remove_user_mode(User& user, const std::string& mode);
  void remove_user_mode(User& user, t_user_mode mode);

  bool user_on_channel(User& user);
  bool user_is_oper(User& user);

  void send_to_channel(User& user, std::string& msg);

  friend bool operator==(const Channel& left, const Channel& right);
  friend bool operator==(const Channel& left, const std::string& channel_name);

 private:
  std::string _name;
  std::string _operator;
  std::string _topic;
  std::string _key;
  size_t      _limit_users;

  std::vector<User *> _users;
  std::vector<User *> _invited;
  std::vector<User *> _banned;

  std::vector<t_channel_mode>                 _modes;
  std::map<User *, std::vector<t_user_mode> > _user_mode;

  std::string _log_message(std::string message);
  void _log(std::string message);

  bool _user_have_mode(User& user, t_user_mode mode);
  void _change_channel_mode(const std::string& user, bool remove);
};

#endif
