#ifndef EXCEPTIONS_H_
# define EXCEPTIONS_H_
# include <exception>
# include <cerrno>
# include <cstring>


struct InvalidArgument : public std::exception {

  virtual const char* what() const throw() {
    return "Invalid argument(s)";
  }

};


struct ErrnoBase : public std::exception {

  virtual const char* what() const throw() {
    return strerror(errno);
  }

};


struct TcpSocketError : public ErrnoBase {};
struct BindError : public ErrnoBase {};
struct ListenError : public ErrnoBase {};


#endif // EXCEPTIONS_H_
