#include <string.h> //Needed for strcmp
#include "io_handler.h"
#include "console/structs.h"

ROM rom={1,0};
void printHelp(char* argv[],int errorCode);

void parseArguments(int argc,char* argv[]){
  char* outROM;

  if(argc<3){
    printHelp(argv,0);
  }else{
    //If no output file name is provided, we need to some fancy string stuff
    if(argc<4){
      int sLength=strlen(argv[2])+1;
      outROM=malloc(sLength+7);
      outROM[0]='\0';

      //Prep outROM, which may contain directory paths
      strncat(outROM,argv[2],sLength);

      //Check for the last ".", working from backwards. "i" needs to be declared outside the for loop, since it'll be used later.
      int i;
      for(i=sLength;i>0;i-=1){
        if(argv[2][i]=='.'){
          break;
        }
      }

      if(i>1){
        outROM[i]='\0';
        strncat(outROM,"_fixed",7);
        strncat(outROM,&argv[2][i],sLength-i); //Copies ".[extension]" to the end of string
                                               //**NOTE** the second argument *has* to be a pointer
      }else{
        strncat(outROM,"_fixed",7);
      }

      printf("outROM: %s\n",outROM);
      free(outROM);
    }else{
      outROM=argv[3];
    }

    openFile(argv[2]);

    if(strcmp(argv[1],"gba")==0){
      processGBAROM(argv[2],outROM);
    }else if(strcmp(argv[1],"gb")==0 || strcmp(argv[1],"gbc")==0 || strcmp(argv[1],"gbx")==0 || strcmp(argv[1],"sgb")==0){
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
  printf("  snes - SNES\n");
}

void openFile(char* fileName){
  FILE* romFile;

  romFile=fopen(fileName,"rb");

  printf("Opening file: \"%s\"\n",fileName);
  if(!romFile){
    printf("Error opening file!\n");
    exit(EXIT_FAILURE);
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

    free(rom.data);

    printf("\nWriting ROM file \"%s\"\n",fileName);
  }else{
    printf("\nData not changed, there's nothing to write\n");
  }
}