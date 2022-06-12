#ifndef IRC_SERVER_H_
# define IRC_SERVER_H_
# include "Socket.hpp"
# include <string>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>

namespace irc {


#define DEFAULT_IP  "127.0.0.1"


class IrcServer {

public:

  IrcServer(const char* port, const char* password);

  void run();

private:

  const std::string _port;
  const std::string _password;
  TcpSocket         _socket;

};


}

#endif // IRC_SERVER_H_
