#include <stdio.h>
#include "sega8.h"
#include "structs.h"

#define CONSOLE_DEFINITION 5

extern void writeFile(char* fileName);

extern ROM rom;

void calculateROMChecksumSEGA8(int size,int headerBase);
int checkSEGA8Header(int headerBase);

unsigned int headerBase=0;

//Region code:
//  3 - Mark III
//  4 - Master System
//  5 - Game Gear, Japan
//  6 - Game Gear, export
//  7 - Game Gear, export (alt)

void processSEGA8ROM(char* inFile,char* outFile){

  const unsigned int headerLocations[3]={0x1ff0,0x3ff0,0x7ff0}
              ,sizeDef[]={
                256*1024  //0
                ,512*1024 //1
                ,0        //2, 1024 Kb, may not work in all BIOS
                ,0        //3
                ,0        //4
                ,0        //5
                ,0        //6
                ,0        //7
                ,0        //8
                ,0        //9
                ,8*1024   //a, If header is at $1ff0
                ,16*1024  //b, If header is at $3ff0
                ,32*1024  //c
                ,0        //d, 48 Kb, may not work in all BIOS
                ,64*1024  //e
                ,128*1024 //f
              };
  unsigned char sizeCode=0
               ,regionCode=0
               ;

  const char invalidROM[]="Header data not found at ROM location 0x1ff0 ,0x3ff0, or 0x7ff0. Most likely not a Mark III/Master System/Game Gear ROM.\n";

  rom.valid=0;

  for(int i=0;i<3 && rom.valid==0;i++){
    if(headerLocations[i]>rom.size){
      break;
    }
    headerBase=headerLocations[i];

    sizeCode=rom.data[headerBase+0xf]&0xf;
    regionCode=rom.data[headerBase+0xf]>>4;

    //Valid region & size codes are needed to make sure headerBase is at the right offset
    if(regionCode>=3 && regionCode<=7 && sizeDef[sizeCode]!=0){
      rom.valid=1;
      break;
    }
  }

  if(rom.valid==1){
    if(regionCode==3){
      printf("This is a Japanese Mark III ROM. There's no checksum to calculate\n");
    }else if(checkSEGA8Header(headerBase)==0){
      if(regionCode==4){
        printf("Processing Master System ROM\n");
        calculateROMChecksumSEGA8(sizeDef[sizeCode],headerBase);
        writeFile(outFile);
      }else if(regionCode==5){
        printf("This is a Japanese Game Gear ROM. There's no checksum to calculate\n");
      }else if(regionCode==6){
        printf("This is a Non-Japanese Game Gear ROM. There's no checksum to calculate\n");
      }else{
        printf("This is a Non-Japanese (alt) Game Gear ROM. There's no checksum to calculate\n");
      }
    }else{
      printf(invalidROM);
    }
  }else{
    printf(invalidROM);
  }

}

void calculateROMChecksumSEGA8(int size,int headerBase){
  checksum16 c={0,rom.data[headerBase+0xa]+(rom.data[headerBase+0xb]<<8)};
  printf("Processing checksum for Master System ROM\n");
  printf("Number of bytes to use: %d bytes\n",size);
  printf("Calculating ROM checksum\n");

  for(int i=0;i<size;i++){
    c.calc+=rom.data[i];
  }

  if(size>=(headerBase+0xf)){
    for(int i=headerBase;i<headerBase+0x10;i++){
      c.calc-=rom.data[i];
    }
  }

  if(c.calc==c.header){
    printf("ROM checksum in header (0x%04x) matches calculated checksum (0x%04x)\n",c.header,c.calc);
  }else{
    printf("ROM checksum in header (0x%04x) does not match calculated checksum (0x%04x)\n",c.header,c.calc);
    printf("Updating data\n");
    rom.data[headerBase+0xa]=(c.calc&0xff);
    rom.data[headerBase+0xb]=(c.calc>>8);
    rom.changed=1;
  }
}

int checkSEGA8Header(int headerBase){
  char sega8Header[]={"TMR SEGA"};
  for(int i=0;i<8;i++){
    if(rom.data[headerBase+i]!=sega8Header[i]){
      return 1;
    }
  }
  return 0;
}