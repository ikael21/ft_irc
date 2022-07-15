#include "utils.hpp"

std::vector<std::string> split(const std::string &str, char delimeter) {
  return split(str, delimeter, std::string::npos);
}


std::vector<std::string> split(const std::string &str, char delimeter, size_t n) {

  std::vector<std::string>  result;
  std::string               item;
  std::istringstream        iss(str);

  while (result.size() < n && std::getline(iss, item, delimeter)) {
    if (!item.length())
      continue;
    result.push_back(item);
  }

  if (std::getline(iss, item, '\0'))
    result.push_back(item);

  return result;
}
