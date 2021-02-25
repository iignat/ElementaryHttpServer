#ifndef SCOCKFDWR_H
#define SCOCKFDWR_H
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int set_nonblock(int fd);
ssize_t sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd);
ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd);

#endif // SCOCKFDWR_H
