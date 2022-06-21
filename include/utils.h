#ifndef UTILS_H_
# define UTILS_H_


/**
 * throws an exception if result equals true
**/
template<class Exception>
void  throw_if_true(bool result)
{ if (result) throw Exception(); }


#endif // UTILS_H_
