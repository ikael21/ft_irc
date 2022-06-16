#include <iostream>
#include <climits>
#include <cstdlib>
#include "IrcServer.hpp"
#include "exceptions.hpp"
#include "utils.h"


/**
 * 1. Checks if number of given arguments is valid,
 * 3 as the first arg is the current program name
 * Also checks if port has only digits
 * and is in range 0...65,535
 */
static void validate_arguments(int argc, char* argv[]) {
  const InvalidArgument& e = InvalidArgument();
  const int number_of_args = 3;
  throw_if_true(argc != number_of_args, e);

  const char* digits = "0123456789";
  const int max_port_length = 5;
  std::string port(argv[1]);
  throw_if_true(port.find_first_not_of(digits) != std::string::npos, e);
  throw_if_true(port.length() > max_port_length, e);
  throw_if_true(atoi(port.c_str()) > USHRT_MAX, e);
}


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
