#include "IrcServer.hpp"
#include "Command.hpp"


typedef void (irc::IrcServer::*handler)(struct kevent& event);


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


bool irc::IrcServer::is_password_correct(std::string pass) {
  return _password == pass;
}


void irc::IrcServer::_accept_handler() {
  struct sockaddr sock_addr;
  socklen_t socklen = sizeof(struct sockaddr);
  int new_fd = accept(_socket, &sock_addr, &socklen);
  throw_if_true<ErrnoBase>(new_fd == -1);

  int res = fcntl(new_fd, F_SETFL, O_NONBLOCK);
  throw_if_true<ErrnoBase>(res == -1);

  User new_user(new_fd);
  struct sockaddr_in* s = (struct sockaddr_in*)&sock_addr;
  new_user.set_hostname(inet_ntoa(s->sin_addr));
  new_user.set_servername(IrcServer::DEFAULT_IP);
  _users.push_back(new_user);

  _add_read_event(_users.back());
  _add_write_event(_users.back());

  // disable to avoid handling of unnecessary events
  _disable_event(new_fd, EVFILT_WRITE);

  #ifdef DEBUG
    _print_new_user_info(new_user);
  #endif
}


void irc::IrcServer::_read_handler(t_event& event) {
  User* user = static_cast<User*>(event.udata);
  user->receive(event.data); // event.data - number of bytes to recieve
  if (user->has_msg()) {
    #ifdef DEBUG
      _print_message_from_user(*user);
    #endif
    _enable_event(*user, EVFILT_WRITE);
  }
  user->set_last_activity(time(NULL));
}


void irc::IrcServer::_write_handler(t_event& event) {
  User* user = (User*)event.udata;
  if (user->get_state() == SEND_PING) {
    _ping_client(*user);
    return;
  }
  if (user->has_msg()) {
    Command(*this, *user, user->get_next_msg()).execute();
    _disable_event(user->get_fd(), EVFILT_WRITE); // TODO add check if all data sent
  }
}


void irc::IrcServer::_execute_handler(t_event& event) {
  static const int conds_num = 4;
  static bool conds[conds_num];

  conds[0] = static_cast<int>(event.ident) == _socket;
  conds[1] = event.flags & EV_EOF;
  conds[2] = event.filter == EVFILT_READ;
  conds[3] = event.filter == EVFILT_WRITE;

  static const handler event_handlers[] = {
    NULL,
    &IrcServer::_delete_client,
    &IrcServer::_read_handler,
    &IrcServer::_write_handler
  };

  if (conds[0])
    return _accept_handler();

  // based on the rest conditions call the suitable handler
  for (int i = 1; i < conds_num; ++i)
    if (conds[i])
      return (this->*event_handlers[i])(event);
}


// TODO refactor
void irc::IrcServer::_check_users_activity() {
  const time_t half_minute = 30;
  for (t_userlist::iterator it = _users.begin(); it != _users.end(); ++it) {

    const time_t time_passed = time(NULL) - it->get_last_activity();
    if (time_passed >= half_minute) {
      bool should_be_deleted = (it->get_status() == AUTHENTICATION ||
                                it->get_state() == WAIT_PONG);
      if (_delete_client_if_true(should_be_deleted, *it))
        continue;
      if (it->get_state() == ACTIVE) {
        it->set_state(SEND_PING);
        _enable_event(*it, EVFILT_WRITE);
      }
    }
  }
}


void irc::IrcServer::run() {
  _add_socket_event();
  _events.reserve(_enabled_events_num);
  while (true) {
    int new_events_num = _wait_for_events();
    _changes.clear(); // clear old event changes

    for (int i = 0; i < new_events_num; ++i)
      _execute_handler(_events[i]);

    _check_users_activity();

    if (!new_events_num) continue;

    _events.reserve(_enabled_events_num);
  }
}


irc::IrcServer::t_channel_list& irc::IrcServer::get_channels() {
  return _channels;
}


void irc::IrcServer::add_channel(Channel& channel) {
  _channels.push_back(channel);
}


/**
 * throws UserNotFound if User is not found in the list
**/
User& irc::IrcServer::get_user_by_nickname(const std::string& nickname) {
  for (t_userlist::iterator i = _users.begin(); i != _users.end(); ++i) {
    if (i->get_nick() == nickname)
      return *i;
  }
  throw UserNotFound();
}
