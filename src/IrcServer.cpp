#include "IrcServer.hpp"


const char* irc::IrcServer::DEFAULT_IP = "127.0.0.1";
const irc::int8_t irc::IrcServer::MAX_QUEUE = 128;
const int irc::IrcServer::INVALID_FD = -1;


irc::IrcServer::IrcServer(const char* port, const char* password)
    : _password(password) {
  _create_socket();
  _initialize_socket(static_cast<irc::port_type>(atoi(port)));
}


irc::IrcServer::~IrcServer() {
  close(_socket);
  close(_kq);
}


void irc::IrcServer::_create_socket() {
  int res;
  const std::exception& e = TcpSocketError();

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  assert_result(_socket == -1, e);

  const int y = 1;
  res = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  assert_result(res == -1, e);

  res = fcntl(_socket, F_SETFL, O_NONBLOCK);
  assert_result(res == -1, e);
}


void irc::IrcServer::_initialize_socket(irc::port_type port) {
  sockaddr_in addr_info;
  int res;

  addr_info.sin_family = AF_INET;
  addr_info.sin_port = htons(port);
  addr_info.sin_addr.s_addr = inet_addr(IrcServer::DEFAULT_IP);

  const sockaddr* addr_info_ptr = (const sockaddr *)&addr_info;
  res = bind(_socket, addr_info_ptr, sizeof(addr_info));
  assert_result(res == -1, BindError());

  res = listen(_socket, MAX_QUEUE);
  assert_result(res == -1, ListenError());
}


void irc::IrcServer::run() {}
