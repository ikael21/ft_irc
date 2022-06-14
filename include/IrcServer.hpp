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

  IrcServer(const IrcServer&);
  IrcServer& operator=(const IrcServer&);

  void _create_socket();
  void _initialize_socket(port_type port);


  const std::string _password;
  int               _socket;
  int               _kq;
  struct kevent     _event_set;


  static const char*  DEFAULT_IP;
  static const int8_t MAX_QUEUE;
  static const int    INVALID_FD;

};


}

#endif // IRC_SERVER_H_
