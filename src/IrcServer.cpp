#include "IrcServer.hpp"

irc::IrcServer::IrcServer(const char* port, const char* password)
    : _port(port), _password(password), _socket() {
  _socket.initialize()
}
