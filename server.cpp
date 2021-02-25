#include "server.h"
#include "scockfdwr.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

#define MAX_EVENTS 128
#define MASTER 0
#define SLAVE 1
static const int SOCKET_COMMON_ERROR = -1;
static const int BUFFER_SIZE = 2048;


server::server()
{

}

int server::add_worker() {
        int z, soc[2],fd;
        char buf[1024];
        pid_t pid;

        z = socketpair (AF_UNIX,SOCK_STREAM,0,soc);

        if ( z == -1 ) {
            std::cerr<< strerror(errno)<<std::endl;
            return -1;
        }

      pid=fork();

      switch (pid){
          case -1:
                  std::cerr<<strerror(errno)<<std::endl;
          break;
          case 0:
              close(soc[MASTER]);

              while(1) {
                  memset(buf,0,sizeof(buf));
                  size_t size = sock_fd_read(soc[SLAVE], buf, sizeof(buf), &fd);
                  if (size >0 && fd!=-1){
                      write(fd, buf, strlen(buf));
                      close(fd);
                  }
             }

          break;
          default:
              close(soc[SLAVE]);

              time_t rawtime;
              struct tm * timeinfo;
              while(1) {
                  time ( &rawtime );
                  timeinfo = localtime ( &rawtime );
                  sprintf (buf, "Current local time and date: %s\0", asctime (timeinfo) );


                  fd=open("/home/box/pairsock.test", O_CREAT | O_WRONLY| O_APPEND);
                  size_t size=sock_fd_write(soc[MASTER],buf,strlen(buf),fd);
                  sleep(1);
              }
          break;
      }
      close(soc[MASTER]);
      close(soc[SLAVE]);
      return 0;
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
            }else{
              char Buffer[BUFFER_SIZE];
              int recs=recv(Events[i].data.fd,Buffer,BUFFER_SIZE,MSG_NOSIGNAL);
              if(recs==0 && errno!=EAGAIN) {
                  shutdown(Events[i].data.fd,SHUT_RDWR);
                  close(Events[i].data.fd);
                }else if(recs>0) {
                  send(Events[i].data.fd,Buffer,recs,MSG_NOSIGNAL);
                  memset(Buffer,0,sizeof(Buffer));
                }
            }
        }
  }

  close(ss);
}
