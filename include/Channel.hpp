#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <stdexcept>
# include <iostream>
# include <sstream>

# include "User.hpp"

# ifndef DEBUG
  # define DEBUG 0
# endif

namespace irc {

/* Список доступных режимов канала:
 *   o - брать/давать привилегии операторов канала
 *   p - флаг приватности канала;
 *   s - флаг секретности канала;
 *   i - флаг канала invite-only;
 *   t - при установке этого флага, менять топик могут только операторы;
 *   n - запрещает сообщения на канал от посторонних клиентов;
 * - m - модерируемый канал;
 *   l - установка ограничения на количество пользователей;
 *   b - установка маски бана;
 * - v - брать/давать возможность голоса при модерируемом режиме;
 *   k - установка на канал ключа (пароля).
 */

typedef enum    s_channel_mode {
  CH_UNKNOWN_MODE       = '\0',
  CH_CHANGE_PRIVILEGE   = 'o',
  CH_PRIVATE            = 'p',
  CH_SECRET             = 's',
  CH_INVITE_ONLY        = 'i',
  CH_TOPIC_MODIFIERS    = 't',
  CH_FORBID_OUT_MSG     = 'n',
  CH_MODERATE           = 'm',
  CH_CHANGE_LIMIT_USERS = 'l',
  CH_BAN_USER           = 'b',
  CH_CHANGE_VOICE       = 'v',
  CH_CHANGE_KEY         = 'k'
}               t_channel_mode;

# define FORBIDDEN_CHANNEL_NAME_SYMBOLS "\t\n "
# define DEFAULT_USERS                  42


class Channel
{
  struct BannedUser
  {
    irc::User*  user;
    irc::User*  who;
    time_t      when;
    BannedUser(irc::User* user, irc::User* who)
      : user(user), who(who), when(std::time(NULL)) {};
  };

 public:
  Channel();
  ~Channel();

  bool is_private();
  bool is_secret();

  void add_user(irc::User& user);
  void remove_user(irc::User& user);

  std::vector<irc::User*> get_users() { return _users; }
  std::vector<irc::User*> get_visible_users();

  void set_name(std::string channelName);
  std::string get_name() { return _name; }

  void set_topic(std::string topic);
  std::string get_topic() { return _topic; }

  void add_channel_mode(t_channel_mode mode);
  void remove_channel_mode(t_channel_mode mode);

  std::vector<t_channel_mode> get_channel_modes() { return _modes; }
  bool have_mode(t_channel_mode mode);

  void add_to_invite_list(irc::User& user);
  void remove_from_invite_list(irc::User& user);
  bool is_invited(irc::User& user);

  void add_to_ban_list(irc::User& user, irc::User& who);
  void remove_from_ban_list(irc::User& user);
  bool is_banned(irc::User& user);
  std::vector<std::string> get_ban_list();

  void set_key(std::string key);
  bool has_key() { return _key.size(); }
  std::string get_key() { return _key; }

  void set_limit_users(int limit);
  int get_limit_users() { return _limit_users; }
  bool is_full() { return _users.size() >= _limit_users; }
  bool is_empty() { return !_users.size(); }

  bool user_on_channel(irc::User& user);

  void add_oper(irc::User& user);
  void remove_oper(irc::User& user);
  bool is_oper(irc::User& user);
  size_t get_num_opers() { return _operators.size(); }

  std::string get_modes_as_str();

  void send_to_channel(irc::User& user, std::string& msg, bool include_myself = true);

  friend bool operator==(const irc::Channel& left, const irc::Channel& right);
  friend bool operator==(const irc::Channel& left, const std::string& channel_name);

 private:
  std::string                 _name;
  std::string                 _topic;
  std::string                 _key;
  size_t                      _limit_users;
  std::vector<irc::User*>     _users;
  std::vector<irc::User*>     _operators;
  std::vector<irc::User*>     _invited;
  std::vector<BannedUser>     _banned;
  std::vector<t_channel_mode> _modes;

  std::string _log_message(std::string message);
  void _log(std::string message);
};

}  // namespace irc

#endif
