#ifndef UTILS_H_
# define UTILS_H_
# include <exception>


/**
 * throws an exception if result equals true
**/
void  assert_result(const bool result,
                    const std::exception& e);


#endif // UTILS_H_
