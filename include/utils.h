#ifndef UTILS_H_
# define UTILS_H_


/**
 * throws an exception if result equals true
**/
template<class Exception>
void  throw_if_true(bool result, const Exception& e)
{ if (result) throw e; }


#endif // UTILS_H_
