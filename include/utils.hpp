#ifndef UTILS_HPP_
# define UTILS_HPP_

# include <string>
# include <sstream>
# include <vector>
# include <algorithm>
# include <list>


// colors for beauty
# define RED     "\033[0;31m"
# define YELLOW  "\033[0;33m"
# define GREEN   "\033[0;32m"
# define MAGENTA "\033[0;35m"
# define BLUE    "\033[0;34m"
# define CYAN    "\033[1;96m"
# define RESET   "\033[0m"


template<class Exception>
void  throw_if_true(bool result)
{ if (result) throw Exception(); }


/**
 * returned array should be freed via delete []
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
std::vector<std::string> split(const std::string &str, char delimeter, size_t n);

bool is_channel(std::string& channel);

#endif // UTILS_HPP_
