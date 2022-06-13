#ifndef INVALIDARGUMENT_H_
# define INVALIDARGUMENT_H_
# include <exception>


struct InvalidArgument : public std::exception {

  virtual const char* what() const throw() {
    return "Invalid argument(s)";
  }

};


#endif // INVALIDARGUMENT_H_
