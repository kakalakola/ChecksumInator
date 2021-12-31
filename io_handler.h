#ifndef IO_HANDLER
  #define IO_HANDLER

  #include <stdio.h>
  #include "console/gbx.h"
  #include "console/gba.h"
  #include "console/sega8.h"

  void parseArguments(int argc,char* argv[]);
  void openFile(char* fileName);
  void writeFile(char* fileName);
#endif