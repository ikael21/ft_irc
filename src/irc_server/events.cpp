#include "IrcServer.hpp"
#include "Command.hpp"

void irc::IrcServer::_add_socket_event() {
  t_event event;
  EV_SET(&event, _socket, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
  _enabled_events_num++;
  _changes.push_back(event);
}


void irc::IrcServer::_add_read_event(User& user) {
  t_event event;
  EV_SET(&event, user.get_fd(), EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, (void*)&user);
  _changes.push_back(event);
  _enabled_events_num++;
}


void irc::IrcServer::_add_write_event(User& user) {
  t_event event;
  EV_SET(&event, user.get_fd(), EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, (void*)&user);
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
void irc::IrcServer::_enable_event(User& user, int type) {
  t_event event;
  EV_SET(&event, user.get_fd(), type, EV_ENABLE, 0, 0, (void*)&user);
  _changes.push_back(event);
  _enabled_events_num++;
}


void irc::IrcServer::_delete_client(t_event& event) {
  int fd = ((User*)event.udata)->get_fd();
  t_userlist::iterator it = _users.begin();
  while (it != _users.end() && it->get_fd() != fd)
    ++it;
  _remove_user_from_channels(*it);
  _users.erase(it);
  close(fd);
  if (_enabled_events_num > 0)
    _enabled_events_num--;

  #ifdef DEBUG
    std::cout << BLUE "User(FD: " << fd
      << ") just left..." RESET << std::endl;
  #endif
}


void irc::IrcServer::delete_client(t_userlist::iterator user) {
  int fd = user->get_fd();
  _remove_user_from_channels(*user);
  _users.erase(user);
  close(fd);
  if (_enabled_events_num > 0)
    _enabled_events_num--;

  #ifdef DEBUG
    std::cout << BLUE "User(FD: " << fd
      << ") just left..." RESET << std::endl;
  #endif
}


int irc::IrcServer::_wait_for_events() {
  int changes_num = static_cast<int>(_changes.size());
  int events_num = static_cast<int>(_enabled_events_num);
  t_event* changes_arr = list_to_array<t_event>(_changes);

  static struct timespec half_minite = { .tv_sec = 30, .tv_nsec = 0 };
  struct timespec* timeout = &half_minite;

#ifdef DEBUG
  std::cout << "waiting for new events(enabled: " RED
    << events_num << RESET ")..." << std::endl;
  _print_event_changes(changes_arr, _changes.size());

  timeout = NULL; // wait until something happens
  (void)half_minite;
#endif

  int new_events_num = kevent(_kq,
                              changes_arr, changes_num,
                              _events.data(), events_num,
                              timeout);
  delete [] changes_arr;

  if (new_events_num == -1)
    std::cerr << ErrnoBase().what() << std::endl;

  return new_events_num;
}
