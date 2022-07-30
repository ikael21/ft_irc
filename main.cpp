#include <iostream>
#include <climits>
#include <cstdlib>
#include "IrcServer.hpp"
#include "exceptions.hpp"
#include "utils.hpp"


/**
 * 1. Checks if number of given arguments is valid,
 * 3 as the first arg is the current program name
 * Also checks if port has only digits
 * and is in range 0...65,535
 */
static void validate_arguments(int argc, char* argv[]) {
  const int number_of_args = 3;
  throw_if_true<InvalidArgument>(argc != number_of_args);

  const char* digits = "0123456789";
  const int max_port_length = 5;
  std::string port(argv[1]);
  throw_if_true<InvalidArgument>(port.find_first_not_of(digits) != std::string::npos);
  throw_if_true<InvalidArgument>(port.length() > max_port_length);
  throw_if_true<InvalidArgument>(atoi(port.c_str()) > USHRT_MAX);
}


/** Might wanna add something like this
 * ./ircserv PORT PASSWORD [ OPTIONS ]
 * --kevent-timeout - sets timeout for kevent sys. call to wait for events (default 1 minute)
 * --ping-after - sets timeout for server to wait before send PING to client (default 1 minute)
 * --pong-delay - sets timeout for server to wait PONG from client (default 1 minute)
**/


int main(int argc, char* argv[]) {
  try {
    validate_arguments(argc, argv);
    irc::IrcServer(argv[1], argv[2]).run();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}
