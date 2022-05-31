#ifndef HTTPRSPCOMPILER_H
#define HTTPRSPCOMPILER_H
#include <unordered_map>
#include "httpreqparser.h"


class httprspcompiler
{
  httpreqparser httpreqps;
public:
  std::string get_current_time();
  std::string get_head(unsigned code);
  std::unordered_map<std::string,std::string> httprsp;
  std::string prepare(unsigned code,std::string data);
  httprspcompiler(httpreqparser _ps):httpreqps(_ps){}
};

#endif // HTTPRSPCOMPILER_H
