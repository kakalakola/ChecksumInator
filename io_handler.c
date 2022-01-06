#include <string.h> //Needed for strcmp
#include <stdlib.h> //Needed for malloc
#include "io_handler.h"
#include "console/structs.h"

#define OUT_ROM_SIZE 255

ROM rom={1,0};
void printHelp(char* argv[],int errorCode);

void parseArguments(int argc,char* argv[]){
  char outROM[OUT_ROM_SIZE+1]="";

  if(argc<3){
    printHelp(argv,0);
  }else{
    if(argc<4){
      strncat(outROM,argv[2],OUT_ROM_SIZE);
    }else{
      strncat(outROM,argv[3],OUT_ROM_SIZE);
    }

    openFile(argv[2]);

    if(strcmp(argv[1],"gba")==0){
      processGBAROM(argv[2],outROM);
    }else if(strcmp(argv[1],"gb")==0 || strcmp(argv[1],"gbx")==0){
      processGBROM(argv[2],outROM);
    }else if(strcmp(argv[1],"ms")==0 || strcmp(argv[1],"sms")==0 || strcmp(argv[1],"gg")==0){
      processSEGA8ROM(argv[2],outROM);
    }else if(strcmp(argv[1],"32x")==0 || strcmp(argv[1],"gen")==0 || strcmp(argv[1],"md")==0){
      processSEGA16ROM(argv[2],outROM);
    }else if(strcmp(argv[1],"snes")==0){
      processSNESROM(argv[2],outROM);
    }else{
      printHelp(argv,1);
    }

    free(rom.data);
  }
}

void printHelp(char* argv[],int errorCode){
  switch(errorCode){
    case 0:
      printf("Invalid number of arguments. ");
      break;
    case 1:
      printf("\nUnknown console: \"%s\"\n",argv[1]);
      break;
  }

  printf("\n");
  printf("Usage: ");
  printf("%s [console] [source] [output(optional)]\n",argv[0]);
  printf("Supported consoles:\n");
  printf("   32x - 32X\n");
  printf("    gb - Game Boy/GB Color/Super GB\n");
  printf("   gba - Game Boy Advance\n");
  printf("   gen - Genesis/Mega Drive\n");
  printf("    md - Genesis/Mega Drive\n");
  printf("    ms - Master System\n");
  printf("  snes - SNES, Lo ROM\n");
  printf("\n");
  printf("If an output file is not specified, and checksum data has been changed, the source file is overwritten.");
}

void openFile(char* fileName){
  FILE* romFile;

  romFile=fopen(fileName,"rb");

  printf("Opening file: \"%s\"\n",fileName);
  if(!romFile){
    printf("Error opening file!\n");
  };

  fseek(romFile,0l,SEEK_END);
  rom.size=ftell(romFile);

  rom.data=malloc(rom.size);

  fseek(romFile,0l,SEEK_SET);
  fread(rom.data,rom.size,1,romFile);

  fclose(romFile);
}

void writeFile(char* fileName){
  if(rom.changed){
    FILE* romFile;
    romFile=fopen(fileName,"wb");
    fwrite(rom.data,1,rom.size,romFile);
    fclose(romFile);

    printf("\nWriting ROM file \"%s\"\n",fileName);
  }else{
    printf("\nData not changed, there's nothing to write\n");
  }
}