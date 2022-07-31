#ifndef IRC_SERVER_H_
# define IRC_SERVER_H_
# include <iostream>
# include <sstream>
# include <string>
# include <algorithm>
# include <cctype>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <vector>
# include <list>
# include "EventsVector.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "exceptions.hpp"
# include "utils.hpp"


namespace irc {


typedef unsigned short  port_type;
typedef unsigned char   int8_t;


class IrcServer {

public:

  typedef struct kevent       t_event;
  typedef std::list<t_event>  t_changelist;
  typedef irc::EventsVector   t_eventlist;
  typedef std::list<User>     t_userlist;
  typedef std::list<Channel>  t_channel_list;


  static const char* DEFAULT_IP;


  IrcServer(const char* port, const char* password);
  ~IrcServer();

  void run();

  bool            is_password_correct(std::string pass);
  void            add_channel(Channel& channel);
  t_channel_list& get_channels();
  User&           get_user_by_nickname(const std::string& nickname);
  void            delete_client(User& user);

  /* Propose to create next methods

  bool isUserOnServer(std::string nick);

  TODO
  возможно лучше создать поле внутри юзера (state/status/is_online)
  сервер будет пинговать юзеров и выставлять поле в зависимости от ответа


  bool isChannelOnServer(std::string channel_name);
  канал автоматически удаляется когда все юзеры покидают данный канал


  Channel& getChannel(std::string channel_name);

  void sendToUser(User& from, User& to, std::string msg);
  void sendToUser(User& from, std::string to_nick, std::string msg);

  void sendToChannel(User& from, Channel& to, std::string msg);
  void sendToChannel(User& from, std::string to_channel, std::string msg);
  */

private:

  IrcServer(const IrcServer&);
  IrcServer& operator=(const IrcServer&);

  void _create_socket();
  void _initialize_socket(port_type port);
  void _initialize_kqueue();

  int  _wait_for_events();
  void _add_socket_event();
  void _add_read_event(User& user);
  void _add_write_event(User& user);
  void _enable_event(User& user, int type);
  void _disable_event(int fd, int type);

  void _execute_handler(t_event& event);
  void _accept_handler();
  void _read_handler(t_event& event);
  void _write_handler(t_event& event);
  void _delete_client(t_event& event);

  void _check_users_activity();

  // helpers

  User& _find_or_create_user(int fd);
  bool _delete_client_if_true(bool result, User& user);
  void _ping_client(User& user);
  void _ping_by_nickname(const User& user);

  // methods for debug (if DEBUG defined)

  void _print_new_user_info(const User& user);
  void _print_user_state(const User& user);
  void _print_message_for_user(const User& user, const std::string& message);
  void _print_message_from_user(User& user);
  void _print_event_changes(t_event* changes, size_t size);


  const std::string _password;
  int               _socket;
  int               _kq;
  t_eventlist       _events;
  t_changelist      _changes;
  size_t            _enabled_events_num;
  t_userlist        _users;
  t_channel_list    _channels;

  static const int8_t MAX_QUEUE;
  static const int    INVALID_FD;

};


} // namespace irc

#endif // IRC_SERVER_H_
