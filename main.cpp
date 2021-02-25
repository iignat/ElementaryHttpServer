#include <iostream>
#include "server.h"

using namespace std;

int main()
{
  server *s=new server();
  s->run();
  return 0;
}
