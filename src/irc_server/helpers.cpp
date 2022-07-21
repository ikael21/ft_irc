#include "IrcServer.hpp"


/**
 * if result true - deletes client from the list and closes connection
 * returns result (first argument)
**/
bool irc::IrcServer::_delete_client_if_true(bool result, User& user) {
  if (result) _delete_client(user);
  return result;
}


User& irc::IrcServer::_find_or_create_user(int fd) {
  for (t_userlist::iterator i = _users.begin(); i != _users.end(); ++i) {
    if (i->get_fd() == fd)
      return *i;
  }
  _users.push_back(User(fd));
  return _users.back();
}


void irc::IrcServer::_ping_client(User& user) {
  _ping_by_nickname(user);
  user.set_state(WAIT_PONG);
  user.set_last_activity(time(NULL));
  _disable_event(user.get_fd(), EVFILT_WRITE);
  #ifdef DEBUG
    _print_user_state(user);
  #endif
}


void irc::IrcServer::_ping_by_nickname(const User& user) {
  std::string message("PING " + user.get_nick() + END_OF_MESSAGE);
  send(user.get_fd(), message.c_str(), message.length(), 0);
  #ifdef DEBUG
    _print_message_for_user(user, message);
  #endif
}
