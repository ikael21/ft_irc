#ifndef UTILS_HPP_
# define UTILS_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <list>

/**
 * throws an exception if result equals true
**/
template<class Exception>
void  throw_if_true(bool result)
{ if (result) throw Exception(); }


/**
 * returned array should be freed via delete [] arr_ptr;
**/
template<class T>
T* list_to_array(const std::list<T>& list) {

  typedef typename std::list<T>::const_iterator iter;

  T* array = new T[list.size()];
  iter list_iter = list.begin();
  size_t arr_pos = 0;
  while (list_iter != list.end()) {
    array[arr_pos] = *list_iter;
    ++arr_pos;
    ++list_iter;
  }
  return array;
}

std::vector<std::string> split(const std::string &str, char delimeter);

#endif // UTILS_HPP_
