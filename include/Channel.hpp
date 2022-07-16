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

  bool isPrivate();

  void addUser(User& user);
  void removeUser(User& user);

  std::vector<User *> getUsers() { return _users; }
  std::vector<User *> getVisibleUsers();

  bool userOnChannel(User& user);

  void setName(std::string channelName);
  std::string getName() { return _name; }

  void setTopic(std::string topic);
  std::string getTopic() { return _topic; }

  void addChannelMode(const std::string& mode);
  void removeChannelMode(const std::string& mode);

  std::vector<t_channel_mode> getChannelModes() { return _modes; }
  bool haveMode(t_channel_mode mode);

  void addToInviteList(User& user);
  bool isInvited(User& user);

  void addToBanList(User& user);
  bool isBanned(User& user);

  void setKey(std::string key);
  bool hasKey() { return _key.size(); }
  bool isCorrectKey(std::string key) { return _key == key; }

  void setLimitUsers(int limit);
  int getLimitUsers() { return _limit_users; }
  bool isFull() { return _users.size() >= _limit_users; }

  void addModeToUser(User& user, const std::string& mode);
  void addModeToUser(User& user, t_user_mode mode);

  void removeUserMode(User& user, const std::string& mode);
  void removeUserMode(User& user, t_user_mode mode);

  bool userIsOper(User& user);

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

  bool _userHaveMode(User& user, t_user_mode mode);
  void _changeChannelMode(const std::string& user, bool remove);
};

#endif
