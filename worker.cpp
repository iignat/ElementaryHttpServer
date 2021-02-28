#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "worker.h"
#include "processor.h"

#define MASTER 0
#define SLAVE 1
#define BUFFER_SIZE 1024

worker::~worker()
{
  //close(soc[MASTER]);
  //close(soc[SLAVE]);
}

worker::worker(std::string _httpdir)
{
      httpdir=_httpdir;
      processor proc=processor(httpdir);
      int z;
      z = socketpair (AF_UNIX,SOCK_STREAM,0,soc);

      if ( z == -1 ) {
            std::cerr<< strerror(errno)<<std::endl;
            return;
      }

      pid=fork();

      switch (pid){
          case -1:
                  std::cerr<<strerror(errno)<<std::endl;
          break;
          case 0:
              close(soc[MASTER]);

              while(1) {
                  char buf[BUFFER_SIZE];
                  int fd;
                  fd_set rfds;
                  FD_ZERO(&rfds);
                  FD_SET(soc[SLAVE], &rfds);
                  select(soc[SLAVE]+1,&rfds,NULL,NULL,NULL);
                  if(!FD_ISSET(soc[SLAVE], &rfds))continue;

                  size_t size = sock_fd_read(buf, sizeof(buf), &fd);//soc[SLAVE]
                  if (size >0 && fd!=-1){
                      int recs=recv(fd,buf,BUFFER_SIZE,MSG_NOSIGNAL);
                      if(recs==0 && errno!=EAGAIN) {
                          shutdown(fd,SHUT_RDWR);
                          close(fd);
                        }else if(recs>0) {

                          std::string ans=proc.process(std::string(buf));
                          std::cout<<"Processed by worker "<<getpid()<<std::endl;
                          std::cout<<"Request:"<<std::endl<<buf<<std::endl;
                          std::cout<<"Response:"<<std::endl<<ans<<std::endl;
                          send(fd,ans.c_str(),ans.length(),MSG_NOSIGNAL);
                          memset(buf,0,sizeof(buf));
                          shutdown(fd,SHUT_RDWR);
                          close(fd);
                       }
                  }
             }

          break;
          default:
              close(soc[SLAVE]);
          break;
      }
      return;
}

ssize_t worker::sock_fd_write(void *buf, ssize_t buflen, int fd) {
    int sock=soc[MASTER];
    ssize_t     size;
    struct msghdr   msg;
    struct iovec    iov;
    union {
        struct cmsghdr  cmsghdr;
        char        control[CMSG_SPACE(sizeof (int))];
    } cmsgu;
    struct cmsghdr  *cmsg;

    iov.iov_base = buf;
    iov.iov_len = buflen;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (fd != -1) {
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_len = CMSG_LEN(sizeof (int));
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;

        std::cout<<"passing fd "<< fd<<std::endl;
        *((int *) CMSG_DATA(cmsg)) = fd;
    } else {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        std::cout<<"not passing fd"<<std::endl;
    }

    size = sendmsg(sock, &msg, 0);

    if (size < 0)
        std::cerr<< "sendmsg:"<<strerror(errno)<<std::endl;
    return size;
}

ssize_t worker::sock_fd_read(void *buf, ssize_t bufsize, int *fd) {
    int sock=soc[SLAVE];
    ssize_t     size;
    if (fd) {
        struct msghdr   msg;
        struct iovec    iov;
        union {
            struct cmsghdr  cmsghdr;
            char        control[CMSG_SPACE(sizeof (int))];
        } cmsgu;
        struct cmsghdr  *cmsg;

        iov.iov_base = buf;
        iov.iov_len = bufsize;

        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        size = recvmsg (sock, &msg, 0);

        if (size < 0) {
            std::cerr<< "recvmsg:"<<strerror(errno)<<std::endl;
            return -1;
        }
        cmsg = CMSG_FIRSTHDR(&msg);

        if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
            if (cmsg->cmsg_level != SOL_SOCKET) {
                std::cerr<< "invalid cmsg_level "<<cmsg->cmsg_level<<std::endl;
                return -1;
            }
            if (cmsg->cmsg_type != SCM_RIGHTS) {
                std::cerr<< "invalid cmsg_type "<<cmsg->cmsg_type<<std::endl;
                return -1;
            }

            *fd = *((int *) CMSG_DATA(cmsg));
            std::cout<<"received fd"<<*fd<<std::endl;
        } else
            *fd = -1;
    } else {
        size = read (sock, buf, bufsize);
        if (size < 0) {
            std::cerr<< "read:"<<strerror(errno)<<std::endl;
            return -1;
        }
    }
    return size;
}
