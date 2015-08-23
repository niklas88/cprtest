#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>
#include <curl/curl.h>
#include <session.h> // since I would use cpr/include as -I this doesn't have cpr in it's name
#include <api.h>     // dito
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"




using namespace rapidjson;

int main(void) {

  auto r = cpr::Get(Url{"http://tourenplaner.informatik.uni-stuttgart.de/info"});
  if(r.status_code == 200){

    Document d;
    if (!d.Parse(r.text.c_str()).HasParseError()) {
      Value& algorithms = d["algorithms"];
      for (SizeType i = 0; i < algorithms.Size(); ++i) {
        const Value& name = algorithms[i]["name"];
        std::cout << "Name: " << name.GetString() << std::endl;
      }
    }
  } else {
    std::cerr << "Status: " << r.status_code << std::endl;
  }
  return 0;
}


