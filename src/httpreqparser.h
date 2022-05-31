#ifndef HTTPREQPARSER_H
#define HTTPREQPARSER_H
#include <vector>
#include <string>
#include <unordered_map>

class strutils
{
public:
  strutils(){}
  static inline void ltrim(std::string &s);
  static inline void rtrim(std::string &s);
  static inline void trim(std::string &s);
  static inline void trim_v(std::vector<std::string> &v);
  static void split(const std::string &str, std::vector<std::string>& v, std::string delimiter);
};

class httpreqparser
{
public:
  std::vector<std::string> httpreqbylines;
  std::unordered_map<std::string,std::string> httpreq;
  httpreqparser(std::string httpreqtxt);
};

#endif // HTTPREQPARSER_H
