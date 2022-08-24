#ifndef IO_HANDLER
  #define IO_HANDLER

  #include <stdio.h>
  #include <stdlib.h> //Needed for malloc and EXIT_SUCCESS/EXIT_FAILURE
  #include "console/gbx.h"
  #include "console/gba.h"
  #include "console/sega8.h"
  #include "console/sega16.h"
  #include "console/snes.h"

  void parseArguments(int argc,char* argv[]);
  void openFile(char* fileName);
  void writeFile(char* fileName);
#endif