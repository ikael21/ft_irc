#ifndef TCPSOCKETERROR_H_
# define TCPSOCKETERROR_H_
# include <exception>
# include <cerrno>
# include <cstring>

struct TcpSocketError : public std::exception {

  virtual const char* what() const throw() {
    return strerror(errno);
  }

};

#endif // TCPSOCKETERROR_H_
