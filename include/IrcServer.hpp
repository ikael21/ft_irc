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
# include <vector>
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

private:

  typedef typename std::vector<struct kevent> t_changelist;
  typedef typename std::vector<struct kevent> t_eventlist;


  IrcServer(const IrcServer&);
  IrcServer& operator=(const IrcServer&);

  void _create_socket();
  void _initialize_socket(port_type port);
  void _initialize_kqueue();
  void _add_read_event(int fd, t_changelist& changelist);
  void _add_write_event(int fd, t_changelist& changelist);


  const std::string _password;
  int               _socket;
  int               _kq;
  t_eventlist       _events;


  static const char*  DEFAULT_IP;
  static const int8_t MAX_QUEUE;
  static const int    INVALID_FD;

};


}

#endif // IRC_SERVER_H_
