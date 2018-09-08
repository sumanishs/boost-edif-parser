#include <iostream>
#include <string>
#include <cstdlib>

#include "EDIFReader.hpp"

using namespace std;


int main(int argc, char* argv[]) {

  if(argc < 2){
    cout << "Usage:./" << argv[0] << "<EDIF file>" << endl;
    exit(1);
  } 

  string file(argv[1]);
  EDIFReader reader;
  reader.Read(file);
  return 0;
}
