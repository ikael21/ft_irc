#ifndef IRC_SERVER_H_
# define IRC_SERVER_H_
# include <iostream>
# include <string>
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
# include "EventsVector.hpp"
# include "User.hpp"
# include "exceptions.hpp"
# include "utils.h"


namespace irc {


typedef unsigned short  port_type;
typedef unsigned char   int8_t;


class IrcServer {

public:

  IrcServer(const char* port, const char* password);
  ~IrcServer();

  void run();

  bool isCorrectPassword(std::string pass);

private:

  typedef struct kevent         t_event;
  typedef std::vector<t_event>  t_changelist;
  typedef irc::EventsVector     t_eventlist;
  typedef std::vector<User>     t_userlist;


  IrcServer(const IrcServer&);
  IrcServer& operator=(const IrcServer&);

  void _create_socket();
  void _initialize_socket(port_type port);
  void _initialize_kqueue();
  int  _wait_for_events(t_changelist &changes);

  void _add_read_event(int fd, t_changelist &changes, uint16_t flags);
  void _add_write_event(int fd, t_changelist &changes, uint16_t flags);

  void _accept_handler(t_changelist& changes);
  void _read_handler(User& user, const t_event& event);
  void _write_handler();

  /** JUST FOR CHECK */
  void _rw_handler(struct kevent);

  User* _find_or_create_user(int fd);


  const std::string _password;
  int               _socket;
  int               _kq;
  t_eventlist       _events;
  size_t            _enabled_events_num;
  t_userlist        _users;

  static const char*  DEFAULT_IP;
  static const int8_t MAX_QUEUE;
  static const int    INVALID_FD;

};


} // namespace irc

#endif // IRC_SERVER_H_
