#include <stdio.h>
#include "gbx.h"
#include "structs.h"

#define GB_NINTENDO_LOGO_SIZE 48

extern void writeFile(char* fileName);

extern ROM rom;

void calculateHeaderChecksumGB();
void calculateROMChecksumGB();


unsigned int nintendoLogoGB[GB_NINTENDO_LOGO_SIZE]={
  0xce,0xed,0x66,0x66,0xcc,0x0d,0x00,0x0b,0x03,0x73,0x00,0x83,0x00,0x0c,0x00,0x0d,0x00,0x08,0x11,0x1f,0x88,0x89,0x00,0x0e,0xdc,0xcc,0x6e,0xe6,0xdd,0xdd,0xd9,0x99,0xbb,0xbb,0x67,0x63,0x6e,0x0e,0xec,0xcc,0xdd,0xdc,0x99,0x9f,0xbb,0xb9,0x33,0x3e
};

void processGBROM(char* inFile,char* outFile){
  for(int i=0;i<GB_NINTENDO_LOGO_SIZE;i++){
    if(rom.data[i+260]!=nintendoLogoGB[i]){
      rom.valid=0;
      break;
    }
  }
  if(rom.valid==1){
    if(rom.data[0x0143]==0 && rom.data[0x0146]==0){
      printf("Processing Game Boy ROM\n");
    }else if(rom.data[0x0143]!=0x80 && rom.data[0x0143]!=0xc0 &&rom.data[0x0146]==3){
      printf("Processing Super Game Boy ROM\n");
    }else if(rom.data[0x0143]==0x80 && rom.data[0x0146]==0){
      printf("Processing Game Boy ROM with Game Boy Color Support\n");
    }else if(rom.data[0x0143]==0x80 && rom.data[0x0146]==3){
      printf("Processing Game Boy ROM with Game Boy Color & Super Game Boy Support\n");
    }else if(rom.data[0x0143]==0xc0 && rom.data[0x0146]==0){
      printf("Processing Game Boy Color ROM\n");
    }
    //There's no need to test for 0xc0 & 0x03 combo, since the Super Game Boy couldn't support GBC's high speed mode.

    calculateHeaderChecksumGB();
    calculateROMChecksumGB();
    writeFile(outFile);
  }else{
    printf("Nintendo logo not found at ROM location 0x0104-0x0133. Most likely not a Game Boy ROM.\n");
  }
}

void calculateHeaderChecksumGB(){
  checksum8 c={0,rom.data[0x14d]};
  printf("Calculating header checksum\n");

  for(int i=0x0134;i<=0x014c;i++){
    c.calc=(c.calc-rom.data[i])-1;
  }

  if(c.calc==c.header){
    printf("Header Checksum in header (0x%02x) matches calculated checksum (0x%02x)\n",c.header,c.calc);
  }else{
    printf("Header Checksum in header (0x%02x) does not match calculated checksum (0x%02x)\n",c.header,c.calc);
    printf("Updating data\n");
    rom.data[0x14d]=c.calc;
    rom.changed=1;
  }
}

void calculateROMChecksumGB(){
  checksum16 c={-(rom.data[0x14e]+rom.data[0x14f]),(rom.data[0x14e]<<8)+(rom.data[0x14f])};
  printf("Calculating ROM checksum\n");

  for(int i=0;i<rom.size;i++){
    c.calc+=rom.data[i];
  }

  if(c.calc==c.header){
    printf("ROM checksum in header (0x%04x) matches calculated checksum (0x%04x)\n",c.header,c.calc);
  }else{
    printf("ROM checksum in header (0x%04x) does not match calculated checksum (0x%04x)\n",c.header,c.calc);
    printf("Updating data\n");
    rom.data[0x14e]=(c.calc>>8);
    rom.data[0x14f]=(c.calc&0xff);
    rom.changed=1;
  }
}