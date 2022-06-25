#include "Command.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

std::vector<std::string> split(const std::string &str, char delimeter) {

  std::vector<std::string>  result;
  std::string               item;
  std::istringstream        iss(str);

  while (std::getline(iss, item, delimeter)) {
    if (!item.length())
      continue;
    result.push_back(item);
  }

  return result;
}

Command :: Command(std::string command) {

  std::replace(command.begin(), command.end(), '\t', ' ');
  _arguments = split(command, ' ');

  if (_arguments.size()) {
    _command = _arguments[0];
    std::transform(_command.begin(), _command.end(), _command.begin(), toupper);
    _arguments.erase(_arguments.begin());
  }
}
