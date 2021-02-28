#include <algorithm>
#include <iostream>
#include "httpreqparser.h"
//============================================ strutil ================================


void strutils::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void strutils::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void strutils::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void strutils::trim_v(std::vector<std::string> &v) {
    for(auto & el:v){
        ltrim(el);
        rtrim(el);
    }
}

void strutils::split(const std::string &str, std::vector<std::string>& v, std::string delimiter) {
    size_t pos = 0;
    std::string token;
    std::string s(str);

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        v.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    v.push_back(s);
}

//==========================================httpreqparse===r===========================
httpreqparser::httpreqparser(std::string httpreqtxt)
{
    std::vector<std::string> v(0);
    strutils::split(httpreqtxt,httpreqbylines,"\r\n");
    strutils::split(httpreqbylines[0],v," ");
    strutils::trim_v(v);
    if(v.size()==3) {
      httpreq["METHOD"]=v[0];
      httpreq["PATH"]=v[1];
      httpreq["PROTO"]=v[2];
     }else{
        std::cout<<"Incorrect HTTP request first line:"<<httpreqbylines[0]<<std::endl;
        httpreq["METHOD"]="GET";
        httpreq["PATH"]="/";
        httpreq["PROTO"]="HTTP/1.0";
     }

    for(unsigned int i=1;i<httpreqbylines.size();i++) {
        if(httpreqbylines[i]=="")continue;
        v.resize(0);
        strutils::split(httpreqbylines[i],v,":");
        for(unsigned k=1;k<v.size();k++) {
            httpreq[v[0]]+=v[k];
            if(k!=v.size()-1)httpreq[v[0]]+=":";
          }
        if(v.size()==1) {
            httpreq["DATA"]+=v[0];
          } else {

              std::string s=httpreq[v[0]];
              strutils::trim(s);
              httpreq[v[0]]=s;
          }
      }
}
