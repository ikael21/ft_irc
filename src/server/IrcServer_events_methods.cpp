#include "IrcServer.hpp"


void irc::IrcServer::_add_socket_event() {
  t_event event;
  EV_SET(&event, _socket, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
  _enabled_events_num++;
  _changes.push_back(event);
}


void irc::IrcServer::_add_read_event(int fd) {
  User& user = _find_or_create_user(fd);
  t_event event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, (void*)&user);
  _changes.push_back(event);
  _enabled_events_num++;
}


void irc::IrcServer::_add_write_event(int fd) {
  User& user = _find_or_create_user(fd);
  t_event event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, (void*)&user);
  _changes.push_back(event);
  _enabled_events_num++;
}

// type (second arg) is either EVFILT_WRITE or EVFILT_READ
void irc::IrcServer::_disable_event(int fd, int type) {
  t_event event;
  EV_SET(&event, fd, type, EV_DISABLE, 0, 0, NULL);
  _changes.push_back(event);
  if (_enabled_events_num > 0)
    _enabled_events_num--;
}

// type (second arg) is either EVFILT_WRITE or EVFILT_READ
void irc::IrcServer::_enable_event(int fd, int type) {
  User& user = _find_or_create_user(fd);
  t_event event;
  EV_SET(&event, fd, type, EV_ENABLE, 0, 0, (void*)&user);
  _changes.push_back(event);
  _enabled_events_num++;
}


void irc::IrcServer::_delete_client(t_event& event) {
  int fd = ((User*)event.udata)->get_fd();
  t_userlist::iterator it = _users.begin();
  while (it != _users.end() && it->get_fd() != fd)
    ++it;
  _users.erase(it);
  close(fd);
  if (_enabled_events_num > 0)
    _enabled_events_num--;

#ifdef DEBUG
  std::cout << BLUE "User(FD: " << fd
    << ") just left..." RESET << std::endl;
#endif
}


int irc::IrcServer::_wait_for_events() {
  struct timespec* timeout = NULL; // wait indefinitely
  int changes_num = static_cast<int>(_changes.size());
  int events_num = static_cast<int>(_enabled_events_num);

#ifdef DEBUG
  std::cout << "waiting for new events(enabled: " RED
    << events_num << RESET ")..." << std::endl;
#endif

  t_event* changes_arr = list_to_array<t_event>(_changes);
  int new_events_num = kevent(_kq,
                              changes_arr, changes_num,
                              _events.data(), events_num,
                              timeout);
  delete [] changes_arr;
  throw_if_true<ErrnoBase>(new_events_num == -1);
  return new_events_num;
}
