#ifndef UTILS_H_
# define UTILS_H_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

/**
 * throws an exception if result equals true
**/
template<class Exception>
void  throw_if_true(bool result)
{ if (result) throw Exception(); }

std::vector<std::string> split(const std::string &str, char delimeter);

#endif // UTILS_H_
