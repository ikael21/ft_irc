#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <algorithm>
# include <stdexcept>
# include <iostream>

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
    CH_CHANGE_PRIVILEGE = 'o',
    CH_PRIVATE = 'p',
    CH_SECRET = 's',
    CH_INVITE_ONLY = 'i',
    CH_TOPIC_MODIFIERS = 't',
    CH_CHANGE_LIMIT_USERS = 'l',
    CH_CHANGE_KEY = 'k'
}               t_channel_mode;

# define FORBIDDEN_CHANNEL_NAME_SYMBOLS "\t\n "
# define DEFAULT_USERS                  42
# define DEFAULT_CHANNEL_MODES          CH_TOPIC_MODIFIERS
# define CHANNEL_MODES                  (CH_PRIVATE, CH_SECRET, CH_INVITE_ONLY, CH_TOPIC_MODIFIERS)

class Channel
{
private:

    std::string _name;
    std::string _operator;
    std::string _topic;
    std::string _key;
    size_t      _limit_users;

    std::vector<t_channel_mode> _modes;

    /* may be in this case we need to use ptrs to objects User */
    std::vector<std::string> _users;
    std::vector<std::string> _invited;

    std::string _log_message(std::string message);
    void _log(std::string message);

public:
    Channel();
    Channel(std::string name, std::string user);

    ~Channel();

    bool isPrivate();

    void addUser(std::string user);
    void removeUser(std::string user);

    std::vector<std::string> getUsers() { return _users; }

    bool userOnChannel(std::string user);

    void setName(std::string name);
    std::string getName() { return _name; }

    void setTopic(std::string topic);
    std::string getTopic() { return _topic; }

    void setChannelMode(std::string mode);
    std::vector<t_channel_mode> getChannelModes() { return _modes; }
    bool haveMode(t_channel_mode mode);

    void addToInviteList(std::string user);
    bool isInvited(std::string user);

    void setKey(std::string key);
    std::string getKey() { return _key; }

    void setLimitUsers(int limit);
    int getLimitUsers() { return _limit_users; }
};

#endif
