#include "IrcServer.hpp"


const char* irc::IrcServer::DEFAULT_IP = "127.0.0.1";
const irc::int8_t irc::IrcServer::MAX_QUEUE = 128;
const int irc::IrcServer::INVALID_FD = -1;


irc::IrcServer::IrcServer(const char* port, const char* password)
    : _password(password), _enabled_events_num(0) {
  _create_socket();
  _initialize_socket(static_cast<irc::port_type>(atoi(port)));
  _initialize_kqueue();
}


irc::IrcServer::~IrcServer() {
  close(_socket);
  close(_kq);
}


void irc::IrcServer::_create_socket() {

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  throw_if_true<TcpSocketError>(_socket == -1);

  int res = 0;
  const int y = 1;
  res = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  throw_if_true<TcpSocketError>(_socket == -1);

  res = fcntl(_socket, F_SETFL, O_NONBLOCK);
  throw_if_true<TcpSocketError>(_socket == -1);
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


User* irc::IrcServer::_find_or_create_user(int fd) {
  for (size_t i = 0; i < _users.size(); ++i) {
    if (_users[i].getFD() == fd)
      return &_users[i];
  }
  _users.push_back(User(fd));
  return &(*(_users.end() - 1));
}


void irc::IrcServer::_add_read_event(int fd,
                                     t_changelist& changes,
                                     uint16_t flags) {

  User* user = _find_or_create_user(fd);
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, flags, 0, 0, (void*)user);
  changes.push_back(event);

  // keep track of enabled events
  bool is_disabled = (flags & EV_DISABLE) || (flags & EV_DELETE);
  if (!is_disabled)
    _enabled_events_num++;
}


void irc::IrcServer::_add_write_event(int fd,
                                      t_changelist& changes,
                                      uint16_t flags) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, flags, 0, 0, NULL);
  changes.push_back(event);

  // keep track of enabled events
  bool is_disabled = (flags & EV_DISABLE) || (flags & EV_DELETE);
  if (!is_disabled)
    _enabled_events_num++;
}


void irc::IrcServer::_accept_handler(t_changelist& changes) {
  struct sockaddr sock_addr;
  socklen_t socklen = sizeof(struct sockaddr);
  int new_fd = accept(_socket, &sock_addr, &socklen);
  throw_if_true<ErrnoBase>(new_fd == -1);

  int res = fcntl(new_fd, F_SETFL, O_NONBLOCK);
  throw_if_true<ErrnoBase>(res == -1);

  _add_read_event(new_fd, changes, EV_ADD | EV_CLEAR);
  _add_write_event(new_fd, changes, EV_ADD | EV_CLEAR | EV_DISABLE);

  _users.push_back(User(new_fd));
}


int irc::IrcServer::_wait_for_events(t_changelist& changes) {
  struct timespec* timeout = NULL; // wait indefinitely
  int changes_num = static_cast<int>(changes.size());
  int events_num = static_cast<int>(_enabled_events_num);
  int new_events_num = kevent(_kq,
                              changes.data(), changes_num,
                              _events.data(), events_num,
                              timeout);
  throw_if_true<ErrnoBase>(new_events_num == -1);
  return new_events_num;
}


void irc::IrcServer::_read_handler(User& user, const t_event& event) {
  char* buffer = new char[event.data + 1];
  ssize_t n_recv = recv(user.getFD(), (void*)buffer, event.data, 0);
  buffer[n_recv] = '\0';

  std::cout << std::string(buffer) << std::endl;
  delete [] buffer;
}


void irc::IrcServer::_write_handler() {
  std::cout << "Write event just happend." << std::endl;
}


void irc::IrcServer::run() {
  t_changelist changes;
  _add_read_event(_socket, changes, EV_ADD | EV_CLEAR); // accept new connections
  _events.reserve(_enabled_events_num);

  while (true) {

    int new_events_num = _wait_for_events(changes);
    if (!new_events_num) continue;

    changes.clear(); // clear old event changes

    for (int i = 0; i < new_events_num; ++i) {
      if (static_cast<int>(_events[i].ident) == _socket) {
        _accept_handler(changes);
      }
      else if (_events[i].filter == EVFILT_READ) {
        _read_handler(*(User*)_events[i].udata, _events[i]);
      }
      else if (_events[i].filter == EVFILT_WRITE) {
        _write_handler();
      }
    }

    // reserve correct memory size for new events
    _events.clear();
    _events.reserve(_enabled_events_num);

  }
}
