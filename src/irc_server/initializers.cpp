#include "IrcServer.hpp"


const char* irc::IrcServer::DEFAULT_IP = "0.0.0.0";
const int irc::IrcServer::INVALID_FD = -1;
const irc::int8_t irc::IrcServer::MAX_QUEUE = 128;


void irc::IrcServer::_create_socket() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  throw_if_true<TcpSocketError>(_socket == -1);

  int res = 0;
  const int y = 1;
  res = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  throw_if_true<TcpSocketError>(_socket == -1);

  res = fcntl(_socket, F_SETFL, O_NONBLOCK);
  throw_if_true<TcpSocketError>(res == -1);
}


void irc::IrcServer::_initialize_socket(irc::port_type port) {
  sockaddr_in addr_info;
  int res;

  addr_info.sin_family = AF_INET;
  addr_info.sin_port = htons(port);
  addr_info.sin_addr.s_addr = inet_addr(IrcServer::DEFAULT_IP);

  const sockaddr* addr_info_ptr = (const sockaddr*)&addr_info;
  res = bind(_socket, addr_info_ptr, sizeof(addr_info));
  throw_if_true<BindError>(res == -1);

  res = listen(_socket, MAX_QUEUE);
  throw_if_true<ListenError>(res == -1);
}


void irc::IrcServer::_initialize_kqueue() {
  _kq = kqueue();
  throw_if_true<ErrnoBase>(_kq == -1);
}
