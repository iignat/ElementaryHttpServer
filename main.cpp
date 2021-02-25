#include <iostream>
#include <fstream>
#include "server.h"

using namespace std;

int main()
{
  ofstream ofs("/tmp/logfile");
  ofstream ofs1("/tmp/errlogfile");
  cout.rdbuf(ofs.rdbuf());
  cerr.rdbuf(ofs1.rdbuf());
  cout << "Goes to file." << endl;

  server *s=new server();
  s->add_worker();
  cout<<"Workers count:"<<s->get_workers_count()<<endl;
  s->run();
  return 0;
}
