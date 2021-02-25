#include <iostream>
#include <fstream>
#include <ctype.h>
#include <unistd.h>
#include "server.h"

using namespace std;

int main(int argc,char ** argv)
{
    string ip = "127.0.0.1";
    unsigned port = 12345;
    string dir="/tmp" ;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "h:p:d:")) != -1)
      switch (c)
        {
        case 'h':
          ip = string(optarg);
          break;
        case 'p':
          port=atoi(optarg);
          break;
        case 'd':
          dir = string(optarg);
          break;
        default:
          abort ();
        }

  ofstream ofs("/tmp/logfile");
  ofstream ofs1("/tmp/errlogfile");
  cout.rdbuf(ofs.rdbuf());
  cerr.rdbuf(ofs1.rdbuf());

  server *s=new server();

  cout<<"Workers count:"<<s->add_worker(dir)<<endl;

  cout<<"Server starts host:"<<ip<<" port:"<<port<<" dir:"<<dir<<endl;
  s->run(ip,port);

  return 0;
}
