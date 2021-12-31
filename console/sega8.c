#include <stdio.h>
#include "sega8.h"

#define CONSOLE_DEFINITION 5

extern void writeFile(char* fileName);
extern unsigned char* data;
extern long long unsigned int dataSize;
extern int validROM,dataChanged;

void calculateROMChecksumSEGA8(int size,int headerBase);
int checkSEGA8Header(int headerBase);

unsigned int headerLocations[]={0x7ff0,0x3ff0,0x1ff0}
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
            }

            ,baseHeaderAddress=0
            ;

unsigned char consoleDefKey[]={3,4,5,6,7};
char* consoleDefValue[]={"Mark III","Master System, export","Game Gear, Japan","Game Gear, export","Game Gear, export (alt)"};

void processSEGA8ROM(char* inFile,char* outFile){
  unsigned char sizeCode=0
               ,regionCode=0
               ;
  //printf("Check for console and region\n");

  //First check for ROM validity, region & ROM size
  validROM=0;
  for(int i=0;i<3 && validROM==0;i++){
    baseHeaderAddress=headerLocations[i];
    //printf("Checking for header at %04x\n",baseHeaderAddress);

    sizeCode=data[baseHeaderAddress+0xf]&0xf;
    regionCode=data[baseHeaderAddress+0xf]>>4;

    //printf("sizeCode: 0x%x\n",sizeCode);
    //printf("regionCode: 0x%x\n",regionCode);
    //printf("\n\n");


    for(int j=0;j<5;j++){
      if(consoleDefKey[j]==regionCode){
        if(j==0){
          validROM=2;
          break;
        }else if(j>1){
          validROM=3;
          break;
        }
        if(sizeDef[sizeCode]!=0){
          validROM=1;
          break;
        }
        break;
      }
    }
  }

  if(validROM==1 && checkSEGA8Header(baseHeaderAddress)==0){
    printf("Processing Master System ROM\n");
    calculateROMChecksumSEGA8(sizeDef[sizeCode],baseHeaderAddress);
    writeFile(outFile);
  }else if(validROM==2){
    printf("This is a Japanese Mark III ROM. There's no checksum to calculate\n");
  }else if(validROM==3 && checkSEGA8Header(baseHeaderAddress)==0){
    printf("This is a Game Gear ROM. There's no checksum to calculate\n");
  }else{
    printf("Header data not found at ROM location 0x1ff0 ,0x3ff0, or 0x7ff0. Most likely not a Mark III/Master System/Game Gear ROM.\n");
  }
}

void calculateROMChecksumSEGA8(int size,int headerBase){
  printf("Processing checksum for Master System ROM\n");
  printf("Amount of data to use: %d bytes\n",size);

  printf("Calculating ROM checksum\n");
    unsigned int sega8ChecksumCalc=0
                 ,sega8Checksum
                 ;

  sega8Checksum=data[headerBase+0xa]+(data[headerBase+0xb]<<8);

  for(int i=0;i<size;i++){
    sega8ChecksumCalc+=data[i];
  }

  //If file size includes area with header data, ignore header data AKA remove the sum of header data from the checksum
  if(size>=(headerBase+0xf)){
    for(int i=headerBase;i<headerBase+0x10;i++){
      sega8ChecksumCalc-=data[i];
    }
  }

  sega8ChecksumCalc=sega8ChecksumCalc&0xffff;

  if(sega8Checksum==sega8ChecksumCalc){
    printf("ROM Checksum in header (0x%04x) matches calculated checksum (0x%04x)\n",sega8Checksum,sega8ChecksumCalc);
  }else{
    printf("ROM Checksum in header (0x%04x) does not match calculated checksum (0x%04x)\n",sega8Checksum,sega8ChecksumCalc);
    printf("Updating data\n");
    data[headerBase+0xa]=(sega8ChecksumCalc&0xff);
    data[headerBase+0xb]=(sega8ChecksumCalc>>8);
    dataChanged=1;
  }
}

int checkSEGA8Header(int headerBase){
  char sega8Header[]={"TMR SEGA"};
  for(int i=0;i<8;i++){
    if(data[headerBase+i]!=sega8Header[i]){
      return 1;
    }
  }
  return 0;
}