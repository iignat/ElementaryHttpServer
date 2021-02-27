#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>

class processor
{
private:
  std::string httpdir;
public:
  inline bool exists (const std::string& name);
  std::string readfile(const std::string& filename);
  std::string process(std::string httpreqtext);
  processor(std::string _httpdir):httpdir(_httpdir){}
};

#endif // PROCESSOR_H
