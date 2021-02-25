#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "server.h"
#include "sockfdwr.h"

#define MAX_EVENTS 128
#define MASTER 0
#define SLAVE 1
static const int SOCKET_COMMON_ERROR = -1;
static const int BUFFER_SIZE = 2048;


server::server()
{
}

int server::set_nonblock(int fd)
{
  int flags;
  #if defined(O_NONBLOCK)
      if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
          flags = 0;
      return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  #else
      flags = 1;
      return ioctl(fd, FIOBIO, &flags);
  #endif

}


int server::add_worker() {
   workers.push_back(worker());
   return get_workers_count();
}

void server::run() {
  int ss = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ss == SOCKET_COMMON_ERROR) {
      std::cerr << "Error on open server socket";
      exit(EXIT_FAILURE);
  }

  struct sockaddr_in sai;
  sai.sin_family = AF_INET;
  sai.sin_port = htons(12345);
  sai.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(ss, (struct sockaddr*) (&sai), sizeof(sai)) == SOCKET_COMMON_ERROR) {
      std::cerr << "Error on bind server socket";
      close(ss);
      exit(EXIT_FAILURE);
  }

  set_nonblock(ss);

  if (listen(ss, SOMAXCONN) == SOCKET_COMMON_ERROR) {
      std::cerr << "Error on start listen server socket" << std::endl;
      close(ss);
      exit(EXIT_FAILURE);
  }
  std::cout << "Ready for connection..." << std::endl;

  int Epoll=epoll_create1(0);
  epoll_event Event;
  Event.data.fd=ss;
  Event.events=EPOLLIN;
  epoll_ctl(Epoll,EPOLL_CTL_ADD,ss,&Event);

  while (true) {
      epoll_event Events[MAX_EVENTS];
      int N=epoll_wait(Epoll,Events,MAX_EVENTS,-1);
      for(unsigned int i=0;i<N;i++) {
          if(Events[i].data.fd==ss) {
              int cs=accept(ss,0,0);
              set_nonblock(cs);
              Event.data.fd=cs;
              epoll_ctl(Epoll,EPOLL_CTL_ADD,cs,&Event);
              std::cout<<"SS event received. EPOLL armed."<<std::endl;
            }else{
              char buff[5];
              std::cout<<"CS event received. Sending to worker."<<std::endl;
              workers[0].sock_fd_write(buff,2,Events[i].data.fd);
              /*char Buffer[BUFFER_SIZE];
              int recs=recv(Events[i].data.fd,Buffer,BUFFER_SIZE,MSG_NOSIGNAL);
              if(recs==0 && errno!=EAGAIN) {
                  shutdown(Events[i].data.fd,SHUT_RDWR);
                  close(Events[i].data.fd);
                }else if(recs>0) {
                  send(Events[i].data.fd,Buffer,recs,MSG_NOSIGNAL);
                  memset(Buffer,0,sizeof(Buffer));
                }*/
            }
        }
  }

  close(ss);
}
