#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

#include "processor.h"
#include "httpreqparser.h"
#include "httprspcompiler.h"

#define RESP404 "<html><head><title>Not Found</title></head><body><H1>404 Not Found.</H1></body></html>"
#define RESP405 "<html><head><title>Method Not Allowed</title></head><body><H1>405 Method Not Allowed.</H1></body></html>"
#define RESP500 "<html><head><title>Internal Server Error</title></head><body><H1>500 Internal Server Error.</H1></body></html>"
#define RESP501 "<html><head><title>Not Implemented</title></head><body><H1>501 Not Implemented.</H1></body></html>"
#define RESP505 "<html><head><title>HTTP Version Not Supported</title></head><body><H1>505 HTTP Version Not Supported.</H1></body></html>"


bool processor::exists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

std::string processor::readfile(const std::string& filename) {
    std::ifstream f(filename);
    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    std::string s(size, ' ');
    f.seekg(0);
    f.read(&s[0], size);
    return s;
}

std::string processor::process(std::string httpreqtext){
  httpreqparser ps=httpreqparser(httpreqtext);
  httprspcompiler rs=httprspcompiler(ps);
  //if(ps.httpreq["PROTO"]!="HTTP/1.0") return rs.prepare(505,RESP505);
  if(ps.httpreq["METHOD"]!="GET" && ps.httpreq["METHOD"]!="HEAD") return rs.prepare(405,RESP405);
  if(ps.httpreq["PATH"]=="/")ps.httpreq["PATH"]="/index.html";
  ps.httpreq["PATH"]=httpdir+ps.httpreq["PATH"];
  if(exists(ps.httpreq["PATH"]))return rs.prepare(200,readfile(ps.httpreq["PATH"]));
      else return rs.prepare(404,RESP404);
  return rs.prepare(500,RESP500);
}

