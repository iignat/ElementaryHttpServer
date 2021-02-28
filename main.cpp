#include <iostream>
#include <fstream>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
  pid_t p=fork();
  switch(p) {
  case 0:
      ofstream ofs("/tmp/logfile");
      ofstream ofs1("/tmp/errlogfile");
      cout.rdbuf(ofs.rdbuf());
      cerr.rdbuf(ofs1.rdbuf());
      server *s=new server();
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      s->add_worker(dir);
      cout<<"Workers count:"<<s->add_worker(dir)<<endl;

      cout<<"Server starts host:"<<ip<<" port:"<<port<<" dir:"<<dir<<endl;
      s->run(ip,port);
      break;
  }
  return 0;
}
