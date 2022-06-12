#include "Socket.hpp"


TcpSocket::TcpSocket() : _socket(INVALID_SOCKET) {}


TcpSocket::~TcpSocket()
{ close(_socket); }


TcpSocket::TcpSocket(const TcpSocket& other) : _socket(other._socket) {}


TcpSocket& TcpSocket::operator=(const TcpSocket& other) {
  close(_socket);
  _socket = other._socket;
  return *this;
}


void TcpSocket::initialize() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket == -1 || fcntl(_socket, F_SETFL, O_NONBLOCK))
    throw;// throw custom exception  (SocketException)
}


int TcpSocket::get_socket() const
{ return _socket; }
