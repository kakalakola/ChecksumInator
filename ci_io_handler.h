#ifndef HEADER_BITS
  #define HEADER_BITS

  #include <cstdlib>
  #include <cstring>
  #include <fstream>
  #include <iostream>
  #include <vector>


  inline const std::string INTRO="BEHOLD THE CHECKSUM-INATOR!!"
  ,VERSION="0.10"
  ,CREDIT="v "+VERSION+" by Saad Azim"
  ,UPDATE="2021 06 03"

  ,COMMON_HEADER=INTRO+"\n"+CREDIT+"\nLast update: "+UPDATE+"\n"
  ;

  inline const std::vector<std::string> supportedConsole{
    //"gen  - SEGA Genesis/Mega Drive"
    //,"md   - SEGA Genesis/Mega Drive"
    //,"snes - Super Famicom/Super NES"
    //,"sneslo - Super Famicom/Super NES, LoROM"
    //,"sneshi - Super Famicom/Super NES, HiROM"
    //,"sfc  - Super Famicom/Super NES"
    //,"gb   - Game Boy"
    //,"gbc  - Game Boy Color"
    //,"sgb  - Super Game Boy"
    "gba  - Nintendo Game Boy Advance"
    ,"ms   - SEGA Master System/Mark III"
  };

  inline const std::vector<std::string> help{"-h","--h","-help","--help","-H","--H","-Help","--Help","-HELP","--HELP"};
  inline const std::vector<std::string> segams{"m3","miii","ms","sms"};
  inline const std::vector<std::string> segagen{"gen","md"};
  inline const std::vector<std::string> snes{"sfc","snes","sns"};

  void printUsage(std::string appName);
  bool checkStrings(char* inChar,const std::vector<std::string>* strings);
  void checkHelp(char* appName,char* inArgument);

  void openFile(char* inFileName,std::vector<unsigned char>* data);
  void saveFile(char* outFileName,std::vector<unsigned char>* outData);

  #include "console/console.h"
  #include "console/gba.h"
  #include "console/sega_8.h"

#endif



