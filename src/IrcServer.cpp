#include "IrcServer.hpp"
#include "Command.hpp"

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
  res ^= 1;
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


void irc::IrcServer::_add_read_event(int fd, t_changelist& changes) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
  changes.push_back(event);
  _enabled_events_num++;
}


void irc::IrcServer::_add_write_event(int fd, t_changelist& changes) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, NULL);
  changes.push_back(event);
  _enabled_events_num++;
}


void irc::IrcServer::_accept_handler(t_changelist& changes) {
  struct sockaddr sock_addr;
  socklen_t socklen = sizeof(struct sockaddr);
  int new_fd = accept(_socket, &sock_addr, &socklen);
  throw_if_true<ErrnoBase>(new_fd == -1);

  int res = fcntl(new_fd, F_SETFL, O_NONBLOCK);
  throw_if_true<ErrnoBase>(res == -1);

  struct kevent event[2];
  EV_SET(&event[0], new_fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
  EV_SET(&event[1], new_fd, EVFILT_WRITE, EV_ADD | EV_CLEAR | EV_DISABLE, 0, 0, NULL);

  changes.push_back(event[0]);
  changes.push_back(event[1]);

  _users.push_back(User(new_fd));
  _enabled_events_num++; // write disabled for now
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


void irc::IrcServer::_read_handler() {
  std::cout << "Read event just happend." << std::endl;
}

void irc::IrcServer::_write_handler() {
  std::cout << "Write event just happend." << std::endl;
}


/** JUST FOR CHECK */
void irc::IrcServer::_rw_handler(struct kevent event) {

  int fd = event.ident;

  for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it) {
    if (*it == fd) {
      std::string res = it->receiveMsg();
      Command cmd = Command(*this, *it, res);
      cmd.excecute();
      break;
    }
  }
}


void irc::IrcServer::run() {
  t_changelist changes;
  _add_read_event(_socket, changes); // accept new connections
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
        _rw_handler(_events[i]);
        // _read_handler();
      }
      else if (_events[i].filter == EVFILT_WRITE) {
        _rw_handler(_events[i]);
        // _write_handler(_events[i]);
      }
    }

    // reserve correct memory size for new events
    _events.reserve(_enabled_events_num);

  }
}

bool irc::IrcServer::isCorrectPassword(std::string pass) {
  return _password == pass;
}
