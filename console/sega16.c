#include <stdio.h>
#include "sega16.h"
#include "structs.h"

#define MAX_ROM_SIZE 4096*1024

extern void writeFile(char* fileName);
extern ROM rom;

void calculateROMChecksumSEGA16();

void processSEGA16ROM(char* inFile,char* outFile){
  rom.valid=0;

  //Header
  //32X headers usually have        "SEGA 32X        " with optional region code at the end
  //Genesis headers usually have    "SEGA GENESIS    "
  //Mega Drive headers usually have "SEGA MEGA DRIVE "
  // - Ball Jacks (JE) has          " SEGA MEGA DRIVE"

  for(int i=0x100;i<0x10c;i++){
    //printf("%c",rom.data[i]);
    if(rom.data[i]=='S'){
      //Check for "SEGA"
      if(rom.data[i+1]=='E' && rom.data[i+2]=='G' && rom.data[i+3]=='A'){
        rom.valid=1;
        break;
      } 
    }
  }
  //printf("\n\n");
  //Check header for valid file
  if(rom.valid){
    printf("Processing SEGA Genesis/Mega Drive/32X ROM\n");
    calculateROMChecksumSEGA16();
    writeFile(outFile);
  }else{
    printf("Header data not found at ROM location 0x100. Most likely not a Genesis/Mega Drive/32X ROM.\n");
  }
}

void calculateROMChecksumSEGA16(){
  checksum16 c={0,(rom.data[0x018e]<<8)+rom.data[0x018f]};
  unsigned int romSize=((rom.data[0x01a4]<<24)+(rom.data[0x01a5]<<16)+(rom.data[0x01a6]<<8)+rom.data[0x01a7])-((rom.data[0x01a0]<<24)+(rom.data[0x01a1]<<16)+(rom.data[0x01a2]<<8)+rom.data[0x01a3])+1;

  //ROM size in header might not match the actual ROM size. From what I've seen, checksums are calculated based on what the size defined in header.

  //A safety check, of sorts, *if* the ROM size defined in header is larger than 4096 Kb
  romSize=romSize>MAX_ROM_SIZE?MAX_ROM_SIZE:romSize;

  for(int i=0x200;i<romSize;i+=2){
    c.calc+=(rom.data[i]<<8);
    c.calc+=rom.data[i+1];
  }

  if(c.calc==c.header){
    printf("ROM checksum in header (0x%04x) matches calculated checksum (0x%04x)\n",c.header,c.calc);
  }else{
    printf("ROM checksum in header (0x%04x) does not match calculated checksum (0x%04x)\n",c.header,c.calc);
    printf("Updating data\n");
    rom.data[0x018e]=(c.calc>>8);
    rom.data[0x018f]=(c.calc&0xff);
    rom.changed=1;
  }
}