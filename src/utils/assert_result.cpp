#include <exception>


void  assert_result(const bool result,
                    const std::exception& e)
{ if (result) throw e; }
