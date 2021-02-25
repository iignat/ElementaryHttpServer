#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"

#define MAX_EVENTS 16
#define MASTER 0
#define SLAVE 1
static const int SOCKET_COMMON_ERROR = -1;
static const int BUFFER_SIZE = 2048;


server::server()
{
}

worker & server::next_worker() {
  this_worker++;
  if(this_worker<workers.begin() || this_worker>workers.end())
      this_worker=workers.begin();
  return *this_worker;
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
   return add_worker("/tmp");
}
int server::add_worker(std::string httpdir){
  workers.push_back(worker(httpdir));
  for(auto &el:workers){
      std::cout<<el.pid<<" ";
    }
  std::cout<<std::endl;
  return get_workers_count();
}

void server::run(std::string host, unsigned port) {
  int ss = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ss == SOCKET_COMMON_ERROR) {
      std::cerr << "Error on open server socket";
      exit(EXIT_FAILURE);
  }

  struct sockaddr_in sai;
  sai.sin_family = AF_INET;
  sai.sin_port = htons(port);
  sai.sin_addr.s_addr = inet_addr(host.c_str());
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

              epoll_event cEvent;
              cEvent.events=EPOLLIN | EPOLLET;
              cEvent.data.fd=cs;

              epoll_ctl(Epoll,EPOLL_CTL_ADD,cs,&cEvent);
            }else{
              char buff[2];
              next_worker().sock_fd_write(buff,1,Events[i].data.fd);
            }
        }
  }

  close(ss);
}
