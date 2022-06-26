#include "utils.h"


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
