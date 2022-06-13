#ifndef SOCKET_H_
# define SOCKET_H_
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>


# define INVALID_SOCKET  -1


class TcpSocket {

public:

  explicit TcpSocket();
  ~TcpSocket();

  TcpSocket(const TcpSocket& other);
  TcpSocket& operator=(const TcpSocket& other);

  void initialize();

  int get_socket() const;

private:

  int _socket;

};

#endif // SOCKET_H_
