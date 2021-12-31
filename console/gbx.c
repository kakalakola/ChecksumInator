#include <stdio.h>
#include "gbx.h"

#define GB_NINTENDO_LOGO_SIZE 48

extern void writeFile(char* fileName);
extern unsigned char* data;
extern long long unsigned int dataSize;
extern int validROM,dataChanged;

void calculateHeaderChecksumGB();
void calculateROMChecksumGB();

unsigned int nintendoLogoGB[GB_NINTENDO_LOGO_SIZE]={
  0xce,0xed,0x66,0x66,0xcc,0x0d,0x00,0x0b,0x03,0x73,0x00,0x83,0x00,0x0c,0x00,0x0d,0x00,0x08,0x11,0x1f,0x88,0x89,0x00,0x0e,0xdc,0xcc,0x6e,0xe6,0xdd,0xdd,0xd9,0x99,0xbb,0xbb,0x67,0x63,0x6e,0x0e,0xec,0xcc,0xdd,0xdc,0x99,0x9f,0xbb,0xb9,0x33,0x3e
};

void processGBROM(char* inFile,char* outFile){
  for(int i=0;i<GB_NINTENDO_LOGO_SIZE;i++){
    if(data[i+260]!=nintendoLogoGB[i]){
      validROM=0;
      break;
    }
  }
  if(validROM==1){
    //To do:
    // - Separate messages depending on GB/SGB/GBC+GB or GBC only ROM...?
    printf("Processing Game Boy/GB Color/Super GB ROM\n");
    calculateHeaderChecksumGB();
    calculateROMChecksumGB();
    writeFile(outFile);
  }else{
    printf("Nintendo logo not found at ROM location 0x0104-0x0133. Most likely not a Game Boy ROM.\n");
  }
}

void calculateHeaderChecksumGB(){
  printf("Calculating header checksum\n");
    unsigned char gbHeaderChecksumCalc=0;

  for(int i=0x0134;i<=0x014c;i++){
    gbHeaderChecksumCalc=(gbHeaderChecksumCalc-data[i])-1;
  }

  if(data[0x14d]==gbHeaderChecksumCalc){
    printf("Header Checksum in header (0x%02x) matches calculated checksum (0x%02x)\n",data[0x14d],gbHeaderChecksumCalc);
  }else{
    printf("Header Checksum in header (0x%02x) does not match calculated checksum (0x%02x)\n",data[0x14d],gbHeaderChecksumCalc);
    printf("Updating data\n");
    data[0x14d]=gbHeaderChecksumCalc;
    dataChanged=1;
  }
}

void calculateROMChecksumGB(){
  printf("Calculating ROM checksum\n");
    unsigned int gbROMChecksumCalc=0
                 ,gbROMChecksum
                 ;

  gbROMChecksum=(data[0x14e]<<8)+(data[0x14f]);

  for(int i=0;i<dataSize;i++){
    gbROMChecksumCalc+=data[i];
  }

  gbROMChecksumCalc=(gbROMChecksumCalc-(data[0x14e]+data[0x14f]))&0xffff;

  if(gbROMChecksum==gbROMChecksumCalc){
    printf("ROM Checksum in header (0x%04x) matches calculated checksum (0x%04x)\n",gbROMChecksum,gbROMChecksumCalc);
  }else{
    printf("ROM Checksum in header (0x%04x) does not match calculated checksum (0x%04x)\n",gbROMChecksum,gbROMChecksumCalc);
    printf("Updating data\n");
    data[0x14e]=(gbROMChecksumCalc>>8);
    data[0x14f]=(gbROMChecksumCalc&0xff);
    dataChanged=1;
  }
}