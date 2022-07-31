#include "IrcServer.hpp"

void irc::IrcServer::_print_new_user_info(const User& user) {
  std::string state(!user.get_state() ? "ACTIVE" : "*_PING");
  std::cout << MAGENTA "New User" << std::endl;
  std::cout << CYAN "\tFD: " YELLOW << user.get_fd() << std::endl;
  std::cout << CYAN "\tHostname: " YELLOW << user.get_hostname() << std::endl;
  std::cout << CYAN "\tState: " GREEN << state << RESET << std::endl;
}


void irc::IrcServer::_print_message_from_user(User& user) {
  std::cout << YELLOW "Message from User(FD: "
    << user.get_fd() << ")" RESET << std::endl;
  std::string tmp(user.get_buffer());
  while (tmp.find(END_OF_MESSAGE) != std::string::npos) {
    std::cout << GREEN "\t|"
      << tmp.substr(0, tmp.find(END_OF_MESSAGE))
      << RESET << std::endl;
    tmp = tmp.substr(tmp.find(END_OF_MESSAGE) +
      std::string(END_OF_MESSAGE).length());
  }
  if (tmp.length())
    std::cout << GREEN "\t|" << tmp << RESET << std::endl;
}


void irc::IrcServer::_print_user_state(const User& user) {
  std::string user_state = (user.get_state() == ACTIVE) ?
    "ACTIVE" : ((user.get_state() == SEND_PING) ?
      "SEND_PING" : "WAIT_PONG");
  std::cout << "Client(FD: " << user.get_fd()
    << ")'s state is " BLUE << user_state << RESET << std::endl;
}


void irc::IrcServer::_print_message_for_user(const User& user,
                                             const std::string& message) {
  std::cout << YELLOW "Reply for User(FD: "
    << user.get_fd() << ")" RESET << std::endl;
  std::cout << GREEN "\t|" << message
    << RESET << std::endl;
}


void irc::IrcServer::_print_event_changes(t_event* changes, size_t size) {
  std::cout << YELLOW "changes:" RESET << std::endl;
  for (size_t i = 0; i < size; ++i) {
    std::string filter(MAGENTA);
    filter += changes[i].filter == EVFILT_WRITE ? "WRITE" : "READ";
    filter += RESET;
    std::string flags(BLUE);
    if (changes[i].flags & EV_ADD) flags += "EV_ADD ";
    if (changes[i].flags & EV_ENABLE) flags += "EV_ENABLE ";
    if (changes[i].flags & EV_DISABLE) flags += "EV_DISABLE ";
    flags += " " RESET;
    User* u = (User*)changes[i].udata;
    std::stringstream ss;
    if (u) {
      ss << "User(" << GREEN << u << RESET << ", FD: "
        << RED << u->get_fd() << RESET ")";
    }
    std::cout << "\t" + filter + "\t" + flags + "\t" + ss.str() << std::endl;
  }
  std::cout << std::endl;
}



