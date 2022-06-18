#include "IrcServer.hpp"


const char* irc::IrcServer::DEFAULT_IP = "127.0.0.1";
const irc::int8_t irc::IrcServer::MAX_QUEUE = 128;
const int irc::IrcServer::INVALID_FD = -1;


irc::IrcServer::IrcServer(const char* port, const char* password)
    : _password(password) {
  _create_socket();
  _initialize_socket(static_cast<irc::port_type>(atoi(port)));
  _initialize_kqueue();
}


irc::IrcServer::~IrcServer() {
  close(_socket);
  close(_kq);
}


void irc::IrcServer::_create_socket() {
  int res;
  const TcpSocketError& e = TcpSocketError();

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  throw_if_true(_socket == -1, e);

  const int y = 1;
  res = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  throw_if_true(res == -1, e);

  res = fcntl(_socket, F_SETFL, O_NONBLOCK);
  throw_if_true(res == -1, e);
}


void irc::IrcServer::_initialize_socket(irc::port_type port) {
  sockaddr_in addr_info;
  int res;

  addr_info.sin_family = AF_INET;
  addr_info.sin_port = htons(port);
  addr_info.sin_addr.s_addr = inet_addr(IrcServer::DEFAULT_IP);

  const sockaddr* addr_info_ptr = (const sockaddr*)&addr_info;
  res = bind(_socket, addr_info_ptr, sizeof(addr_info));
  throw_if_true(res == -1, BindError());

  res = listen(_socket, MAX_QUEUE);
  throw_if_true(res == -1, ListenError());
}


void irc::IrcServer::_initialize_kqueue() {
  _kq = kqueue();
  throw_if_true(_kq == -1, ErrnoBase());
}


void irc::IrcServer::_add_read_event(int fd, t_changelist& changelist) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
  changelist.push_back(event);
}


void irc::IrcServer::_add_write_event(int fd, t_changelist& changelist) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, NULL);
  changelist.push_back(event);
}


void irc::IrcServer::run() {
  struct timespec* timeout = NULL; // wait indefinitely
  t_changelist changes;

  _add_read_event(_socket, changes);
  _events.reserve(changes.size());
  while (true) {
    int events_num = kevent(_kq,
        changes.data(), static_cast<int>(changes.size()),
        _events.data(), static_cast<int>(_events.size()), timeout);
    throw_if_true(events_num == -1, ErrnoBase());

    /*
      NOTE: events handling

      FOR event IN events
        event.udata.handle_event

      NOTE:
      change size of events according to
      new assigned changes and old relevant events

      size_t old_size = _events.size();
      _events.reserve(old_size + changes.size());

      NOTE:
      Decrease capacity of _events

    */
  }
}
