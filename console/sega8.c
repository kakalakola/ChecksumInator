#include <stdio.h>
#include "sega8.h"
#include "structs.h"

extern void writeFile(char* fileName);
extern ROM rom;

void calculateROMChecksumSEGA8(int size,int headerBase);
unsigned int checkSEGA8Header(int headerBase); //Checks for the presence of "TMR SEGA" 
unsigned int checkROMSize(int sizeCode); //Checks for valid ROM size in header

unsigned int headerBase=0;

//Region code:
//  3 - Mark III
//  4 - Master System
//  5 - Game Gear, Japan
//  6 - Game Gear, export
//  7 - Game Gear, export (alt)

void processSEGA8ROM(char* inFile,char* outFile){

  const unsigned int headerLocations[3]={0x1ff0,0x3ff0,0x7ff0}
              ;
  unsigned char sizeCode=0
               ,regionCode=0
               ,sega8HeaderCheck=1
               ;

  const char invalidROM[]="Header data not found at ROM location 0x1ff0 ,0x3ff0, or 0x7ff0. Most likely not a Mark III/Master System/Game Gear ROM.\n";

  rom.valid=0;

  if(rom.size<0x8000){
    printf("File is too small (%lld bytes) to be a valid Mark III/Master System/Game Gear ROM.\n",rom.size);
  }else{

    for(int i=0;i<3;i++){
      sega8HeaderCheck=checkSEGA8Header(headerLocations[i]);
      if(sega8HeaderCheck==0){
        headerBase=headerLocations[i];
        break;
      }
    }
  
    if(sega8HeaderCheck!=0){
      printf(invalidROM);
    }else{
  
      sizeCode=rom.data[headerBase+0xf]&0xf;
      regionCode=rom.data[headerBase+0xf]>>4;
  
      //Valid region & size codes are needed to make sure headerBase is at the right offset
      if(regionCode>=3 && regionCode<=7 && rom.valid==0){
        rom.valid=1;
      }
  
      if(rom.valid==1){
        if(regionCode==3){
          printf("This is a Japanese Mark III ROM. There's no checksum to calculate\n");
        }else if(sega8HeaderCheck==0){
          if(regionCode==4){
            printf("Processing Master System ROM\n");
            calculateROMChecksumSEGA8(checkROMSize(sizeCode),headerBase);
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
  }

}

void calculateROMChecksumSEGA8(int size,int headerBase){
  checksum16 c={0,rom.data[headerBase+0xa]+(rom.data[headerBase+0xb]<<8)};

  unsigned int headerSkip=0; //Need a separate variable to trak what to skip in header
                             //"headerBase" is needed to write the modified checksum back into the ROM

  //Turns out header calculation is a little more complicated than I thought. Specific bytes need to be skipped regardless of where the actual header data is.
  if(size<=0xc000){
    headerSkip=size-0x10;
  }else{
    headerSkip=0x7ff0; //For ROMs larger than 48 Kb, 0x10 bytes starting at 0x7FF0 are ALWAYS skipped
                       //Regardless of where the actual header is
  }

  for(int i=0;i<size;i++){
    c.calc+=rom.data[i];
  }

 for(int i=headerSkip;i<headerSkip+0x10;i++){
   c.calc-=rom.data[i];
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

unsigned int checkSEGA8Header(int headerBase){
  char sega8Header[]={"TMR SEGA"};

  for(int i=0;i<8;i++){
    if(rom.data[headerBase+i]!=sega8Header[i]){
      return 1;
    }
  }
  return 0;
}

unsigned int checkROMSize(int sizeCode){
  switch(sizeCode){
    case 0:
      return 0x40000; //256 Kb;
    case 1:
      return 0x80000; //512 Kb
    case 2:
      return 0x100000; //1024 Kb - unused, buggy (?)
    case 0xa:
      return 0x1000; //4 Kb, unused (?)
    case 0xb:
      return 0x4000; //16 Kb - unused (?)
    case 0xc:
      return 0x8000; //32 Kb
    case 0xd:
      return 0xC000; //48 Kb - unused, buggy (?)
    case 0xe:
      return 0x10000; //64 Kb - rarely used
    case 0xf:
      return 0x20000; //128 Kb
  };
  return 0;
}
