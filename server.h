#ifndef SERVER_H
#define SERVER_H
#include <vector>
#include "worker.h"


class server
{
private:
   std::vector<worker> workers;
public:
  server();
  void run();
  int add_worker();
  size_t get_workers_count(){return workers.size();}
  static int set_nonblock(int fd);
};

#endif // SERVER_H
