#ifndef SOCKFDWR_H
#define SOCKFDWR_H
#include <sys/types.h>

class sockfdwr
{
public:
  sockfdwr();
  static int set_nonblock(int fd);
  //static ssize_t sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd);
  //static ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd);
};

#endif // SOCKFDWR_H
