#ifndef WORKER_H
#define WORKER_H
#include <sys/socket.h>
#include <sys/types.h>

class worker
{
public:
  int soc[2];
  pid_t pid;
  worker();
  ~worker();
  ssize_t sock_fd_read(void *buf, ssize_t bufsize, int *fd);
  ssize_t sock_fd_write(void *buf, ssize_t buflen, int fd);
};

#endif // WORKER_H
