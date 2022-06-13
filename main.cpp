#include <iostream>
#include "IrcServer.hpp"
#include "exceptions.hpp"
#include <climits>
#include <cstdlib>


/**
 * 1. Checks if number of given arguments is valid,
 * 3 as the first arg is the current program name
 * Also checks if port has only digits
 * and is in range 0 - 65,535
 */
static void validate_arguments(int argc, char* argv[]) {
  if (argc != 3) throw InvalidArgument();

  const char* digits = "0123456789";
  const int max_port_length = 5;
  std::string port(argv[1]);

  if (port.find_first_not_of(digits) != std::string::npos ||
      port.length() > max_port_length ||
      atoi(port.c_str()) > USHRT_MAX) {
    throw InvalidArgument();
  }
}


int main(int argc, char* argv[]) {
  try {
    validate_arguments(argc, argv);
    irc::IrcServer server(argv[1], argv[2]);
    server.run();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}
