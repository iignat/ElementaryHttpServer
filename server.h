#ifndef SERVER_H
#define SERVER_H
#include <vector>
#include <cstring>
#include "worker.h"


class server
{
private:
   std::vector<worker> workers;
   std::vector<worker>::iterator this_worker;
   worker & next_worker();
public:
  server();
  void run(std::string host, unsigned port);
  int add_worker();
  int add_worker(std::string httpdir);
  size_t get_workers_count(){return workers.size();}
  static int set_nonblock(int fd);
};

#endif // SERVER_H
