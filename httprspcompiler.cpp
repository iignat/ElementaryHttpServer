#include "httprspcompiler.h"
#include <ctime>
#include <cstdlib>

std::string httprspcompiler::get_head(unsigned code) {
  std::string s=httpreqps.httpreq["PROTO"]+" "+std::to_string(code)+" ";
  switch(code) {
    case 200:
      s=s+"OK"+"\r\n";
      break;
    case 404:
      s=s+"Not Found"+"\r\n";
      break;
    case 405:
      s=s+"Method Not Allowed"+"\r\n";
      break;
    case 501:
      s=s+"Not Implemented"+"\r\n";
      break;
    case 505:
      s=s+"HTTP Version Not Supported"+"\r\n";
      break;
     default:
      s=httpreqps.httpreq["PROTO"]+" 500 Internal Server Error\r\n";
    }

  return s;
}
std::string httprspcompiler::prepare(unsigned code,std::string data){
  std::string s="";
  s=get_head(code);
  s+="Date: "+get_current_time()+"\r\n";
  s+="Last-Modified: "+get_current_time()+"\r\n";
  s+="Connection: close \r\n";
  s+="Server: Elementary Http Server \r\n";
  s+="Content-Type: text/html; charset=UTF-8 \r\n";
  s+="Content-Length: "+std::to_string(data.length())+" \r\n\r\n";  
  if(httpreqps.httpreq["METHOD"]!="HEAD")s+=data;
  s+="\r\n\r\n";

  return s;
}

std::string httprspcompiler::get_current_time()
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char output[30]={0};
    strftime(output, 30, "%a, %d %m %Y %H:%M:%S GTM", timeinfo);

    return std::string(output);
}

