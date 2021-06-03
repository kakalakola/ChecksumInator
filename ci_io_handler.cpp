#include "ci_io_handler.h"

void parseArguments(char* inConsole,char* inFile){
  std::cout<<"Parsing arguments:\n  Console: "<<inConsole<<"\n  File: "<<inFile;
}

//Needs to be defined before check help
void printUsage(std::string appName){
  std::cout<<"Usage:\n";
  std::cout<<appName<<" [console] [input file]\n";

  std::cout<<"\nSupported consoles:";
  for(const auto & console:supportedConsole){
    std::cout<<"\n  "<<console;
  }
  exit(0);
}

bool checkStrings(char* inChar,const std::vector<std::string>* strings){
  //It might be more economical to convert char* to an std::string in this function
  std::string inString=inChar;
  bool result=false;
  for(auto string:*strings){
    if((inString).compare(string)==0){
      result=true;
    }
  }
  return result;
}

//Checking if the help argument has been used
void checkHelp(char* appName,char* inArgument){
  if(checkStrings(inArgument,&help)){
    std::cout<<"...helping\n";
    printUsage(appName);
  }
}

//Moral of this story. Do NOT use std::byte for reading a binary file. std::byte was NOT meant for this. unsigned char works MUCH better... >.<
void openFile(char* inFileName,std::vector<unsigned char>* data){
  std::ifstream inFile(inFileName,std::ios::in|std::ios::binary);

  if(!inFile){
    throw std::runtime_error(std::strerror(errno));
  }

  inFile.seekg(0,std::ios::end);
  auto fileEnd=inFile.tellg();
  inFile.seekg(0,std::ios::beg);
  unsigned int size=std::size_t(fileEnd-inFile.tellg());

  (*data).resize(size);

  if(size==0){
    throw std::runtime_error("File is 0 bytes in size");
  }

  if(!inFile.read((char*)(*data).data(),size)){
    throw std::runtime_error(std::strerror(errno));
  }

  inFile.close();
}

void saveFile(char* outFileName,std::vector<unsigned char>* data){
  std::cout<<"\nSaving changes to: \""<<outFileName<<"\"\n";

  std::ofstream outFile;
  unsigned int size=(*data).size();
  outFile.open(outFileName,std::ios::out|std::ios::binary);

  if(!outFile){
    throw std::runtime_error("Unable to create file");
  }

  if(!outFile.write((char *)(*data).data(),size)){
    throw std::runtime_error("Unable to write data");
  }

  outFile.close();

  std::cout<<"Wrote "<<size<<" bytes ("<<(size>>10)<<" Kb) to file\n";
}