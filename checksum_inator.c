//#include <stdio.h>

#include "io_handler.h"

#define VERSION "0.8.0"
#define BUILD_DATE "2022.01.04"


int main(int argc,char* argv[]){
  printf("BEHOLD! THE CHECKSUM INATOR!!\n");
  printf("v%s by: Saad Azim\n",VERSION);
  printf("Build date: %s\n",BUILD_DATE);
  printf("\n");

  parseArguments(argc,argv);

  return 0;
}