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


User& irc::IrcServer::_find_or_create_user(int fd) {
  for (t_userlist::iterator i = _users.begin(); i != _users.end(); ++i) {
    if (i->get_fd() == fd)
      return *i;
  }
  _users.push_back(User(fd));
  return _users.back();
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
  std::string state(!new_user.get_state() ? "ACTIVE" : "*_PING");
  std::cout << MAGENTA "New User" << std::endl;
  std::cout << CYAN "\tFD: " YELLOW << new_fd << std::endl;
  std::cout << CYAN "\tHostname: " YELLOW << new_user.get_hostname() << std::endl;
  std::cout << CYAN "\tState: " GREEN << state << RESET << std::endl;
#endif
}


void irc::IrcServer::_read_handler(t_event& event) {
  User* user = static_cast<User*>(event.udata);
  user->receive(event.data); // event.data -> number of bytes to recieve

  if (user->has_msg()) {

    // TODO check PONG message if state - WAIT_PONG
    // set state ACTIVE if all good

    _enable_event(*user, EVFILT_WRITE);

#ifdef DEBUG
    std::cout << YELLOW "Message from User(FD: "
      << user->get_fd() << ")" RESET << std::endl;

    std::string tmp(user->get_buffer());
    while (tmp.find(END_OF_MESSAGE) != std::string::npos) {
      std::cout << GREEN "\t|"
        << tmp.substr(0, tmp.find(END_OF_MESSAGE))
        << RESET << std::endl;
      tmp = tmp.substr(tmp.find(END_OF_MESSAGE) + 1);
    }
    if (tmp.length())
      std::cout << GREEN "\t|" << tmp << RESET << std::endl;
#endif
  }
}


void irc::IrcServer::_write_handler(t_event& event) {
  User* user = (User*)event.udata;

  if (user->get_state() == SEND_PING) {
    _ping_by_nickname(*user);
    user->set_state(WAIT_PONG);
    _disable_event(user->get_fd(), EVFILT_WRITE);
  }
  else if (user->has_msg()) {
    Command command = Command(*this, *user, user->get_next_msg());
    command.execute();

    // TODO if all data sent, need to disable event notify
    _disable_event(user->get_fd(), EVFILT_WRITE);
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


void irc::IrcServer::_check_users_activity() {
  const time_t minute = 60;
  for (t_userlist::iterator it = _users.begin(); it != _users.end(); ++it) {
    const time_t time_passed = time(NULL) - it->get_last_activity();
    if (time_passed >= minute) {
      if (it->get_state() == ACTIVE) {
        it->set_state(SEND_PING);
        _enable_event(*it, EVFILT_WRITE);

        #ifdef DEBUG
          std::cout << "Client(FD: " << it->get_fd()
            << ")'s state set to " BLUE "SEND_PING" << RESET << std::endl;
        #endif
      }
      else if (it->get_state() == WAIT_PONG) {
        _delete_client(*it);

      }
    }

  }
}


void irc::IrcServer::run() {
  _add_socket_event(); // accept incoming connections
  _events.reserve(_enabled_events_num);

  while (true) {
    int new_events_num = _wait_for_events();
    _changes.clear(); // clear old event changes
    for (int i = 0; i < new_events_num; ++i)
      _execute_handler(_events[i]);

    /* TODO
      Check last accepted event time for all users,
      if there's much time passed - ping user (and wait answer for 1 minute)

      Add some kinda states for user: ACTIVE, WAIT_PING, WAIT_PONG
      ACTIVE -> user's online and sends messages
      WAIT_PING -> ping user to see if he's still online, state is now WAIT_PONG
      WAIT_PONG -> server waits message from user
    */

   _check_users_activity();

    // reserve correct memory size for new events
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


void irc::IrcServer::_ping_by_nickname(const User& user) {
  std::string message("PING " + user.get_nick() + "\r\n");
  send(user.get_fd(), message.c_str(), message.length(), 0);

#ifdef DEBUG
  std::cout << YELLOW "Reply for User(FD: "
    << user.get_fd() << ")" RESET << std::endl;
  std::cout << GREEN "\t|" << message
    << RESET << std::endl;
#endif
}
