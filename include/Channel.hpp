#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <stdexcept>
# include <iostream>

# include "User.hpp"

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
private:
  std::string _name;
  std::string _operator;
  std::string _topic;
  std::string _key;
  size_t      _limit_users;


  /* may be in this case we need to use ptrs to objects User */
  std::vector<std::string> _users;
  std::vector<std::string> _invited;

  std::vector<t_channel_mode>                      _modes;
  std::map<std::string, std::vector<t_user_mode> > _user_mode;

  std::string _log_message(std::string message);
  void _log(std::string message);

  bool _userHaveMode(std::string username, t_user_mode);
  void _changeChannelMode(std::string mode, bool remove);

public:
    Channel();
    Channel(std::string channelName, std::string username);

    ~Channel();

    bool isPrivate();

    void addUser(std::string username);
    void removeUser(std::string username);

    std::vector<std::string> getUsers() { return _users; }
    std::vector<std::string> getVisibleUsers();

    bool userOnChannel(std::string username);

    void setName(std::string channelName);
    std::string getName() { return _name; }

    void setTopic(std::string topic);
    std::string getTopic() { return _topic; }

    void addChannelMode(std::string mode);
    void removeChannelMode(std::string mode);

    std::vector<t_channel_mode> getChannelModes() { return _modes; }
    bool haveMode(t_channel_mode mode);

    void addToInviteList(std::string username);
    bool isInvited(std::string username);

    void setKey(std::string key);
    std::string getKey() { return _key; }

    void setLimitUsers(int limit);
    int getLimitUsers() { return _limit_users; }

    void addModeToUser(std::string username, std::string mode);
    void removeUserMode(std::string username, std::string mode);

    bool userIsOper(std::string username);
};

#endif
