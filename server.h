#ifndef SERVER_H
#define SERVER_H
#include <vector>


class server
{
private:
   std::vector<int[2]> pipes;
public:
  server();
  void run();
  int add_worker();
};

#endif // SERVER_H
