#include <stdio.h>
#include "snes.h"
#include "structs.h"

extern void writeFile(char* fileName);
extern ROM rom;

int checkMapMode(int mapMode);
int checkROMType(int romType);
void calculateROMChecksumSNES(int headerBase,int mapMode);

void processSNESROM(char* inFile,char* outFile){
  const unsigned int headerLocations[4]={0x7f00,0xff00,0x407f00,0x40ff00};
  const char* headerDef[]={"LoROM","HiROM","Ex LoROM","Ex HiROM"};

  unsigned int headerBase=0
               ,mapMode=0
               ,i=0
               ;

  //printf("Processing SNES ROM\n");
  rom.valid=0;

  for(i=0;i<4;i++){

    if(headerLocations[i]>rom.size){
      break;
    }
 
    headerBase=headerLocations[i];
    mapMode=rom.data[headerBase+0xd5];

    //First pass to make sure that rom configuration data at 0x7fd5, 0xffd5, 0x407fd5 or 0x40ffd5 isn't a valid ROM configuration value by chance.
    if(
      checkMapMode(rom.data[headerBase+0xd5])
      && checkROMType(rom.data[headerBase+0xd6])
      && rom.data[headerBase+0xd7]>6 && rom.data[headerBase+0xd7]<0x0e
      && rom.data[headerBase+0xd8]<8
      && rom.data[headerBase+0xd9]<0x15
    ){
      rom.valid=1;
      //Assuming header data at 0x7fd0 or 0xffd0 is valid, there's *still* a chance that the ROM might be an Ex LoROM/Ex HiROM

      if((mapMode==0x25 || mapMode==0x35) && i<2){
        i=mapMode>>4;

        //Safety check, again
        if(headerLocations[i]>rom.size){
          break;
        }

        printf("Map mode: 0x%02x - Checking for header data at 0x%08x\n",mapMode,headerLocations[i]);
        if(
            checkMapMode(rom.data[headerLocations[i]+0xd5])
            && checkROMType(rom.data[headerLocations[i]+0xd6])
            && rom.data[headerLocations[i]+0xd7]>6 && rom.data[headerLocations[i]+0xd7]<0x0e
            && rom.data[headerLocations[i]+0xd8]<8
            && rom.data[headerLocations[i]+0xd9]<0x15
          ){
          headerBase=headerLocations[i];
          break;
        }
      }
      break;
    }
  }

  if(rom.valid){
    printf("Processing SNES %s\n",headerDef[i]);
    calculateROMChecksumSNES(headerBase,mapMode);
    writeFile(outFile);
  }else{
    printf("Valid SNES ROM header not found. This might not be an SNES ROM.\n");
  }
}

int checkMapMode(int mapMode){
  //Bulkier than parsing FOR loops, but *slightly* faster. And easier to comment on.
  switch(mapMode){
    case 0x20: //Mode 20 slow LoROM
    case 0x21: //Mode 21 slow HiRom
    case 0x22: //Reserved for future slow LoROM use ...?
    case 0x23: //Mode 23 SA-1+slow LoROM
    case 0x25: //Mode 25 slow HiROM (Ex LoROM...?)
    case 0x30: //Mode 30 fast LoRom
    case 0x31: //Mode 31 fast HiROM
    case 0x32: //Mode 32 S-DD1 fast LoROM (used by Star Ocean & SFA 2)
    case 0x33: //Mode 23 SA-1+fast LoROM ...?
    case 0x35: //Mode 35 Ex HiROM
    case 0x3a: //Fast HiROM - Used by palsoft games? Games that use SPC7110
      return 1;
  }
  return 0;
};


int checkROMType(int romType){
  //Pass 01, check if it's a regular ROM with extra RAM and/or SRAM
  if(romType<3){
    return 1;
  }else{
    //Pass 02, try to parse the ROM type code
    switch(romType>>4){
      case 0x0: //0x0n DSP
      case 0x1: //0x1n GSU
      case 0x2: //0x2n OBC1
      case 0x3: //0x3n SA-1
      case 0x4: //0x4n S-DD1
      case 0x5: //0x5n S-RTC
      case 0xe: //0xen Other (SGB/BS)
      case 0xf: //0xfn Custom (with extra bits at 0xffbf) 
        romType&=0x0f;
        break;
      default:
        return 0;
    }

    switch(romType){
      case 3: //ROM+Co-processor
      case 4: //ROM+Co-processor+RAM
      case 5: //ROM+Co-processor+RAM+Battery
      case 6: //ROM+Co-processor+Battery
      case 9: //ROM+Co-processor+RAM+Battery+RTC-4513
        return 1;
    }
  }
  return 0;
}

void calculateROMChecksumSNES(int headerBase,int mapMode){
  checksum16 c={
                0x1fe -(rom.data[headerBase+0xdc]+rom.data[headerBase+0xdd]+rom.data[headerBase+0xde]+rom.data[headerBase+0xdf])
                ,rom.data[headerBase+0xde]+(rom.data[headerBase+0xdf]<<8)
              }
              ,cc={
                0
                ,rom.data[headerBase+0xdc]+(rom.data[headerBase+0xdd]<<8)
              }
              ;
  unsigned int romSizeHeader=2<<(rom.data[headerBase+0xd7]+9);

  //Common to all ROM checksum
  //End of calculation for:
  // rom.size == ROM size in header
  // ROM mapping 0x3a
  for(int i=0;i<rom.size;i++){
    c.calc+=rom.data[i];
  }

  //Momotaru Densetsu Happy requires the checksum to be multiplied by 2 for ... reasons...?
  if(mapMode==0x3a && rom.data[headerBase+0xd7]<0x0d && rom.size<romSizeHeader){
    c.calc<<=1;

  //One last check ROM size extensions only apply to roms larger than 128 Kb (the smallest valid size)
  //Or the mapping mode isn't 0x3a
  }else if(rom.size<romSizeHeader && rom.size>0x20000 && mapMode!=0x3a ){
    int dataRepeatAmount=romSizeHeader-rom.size
        ,dataRepeat=romSizeHeader
        ,dataRepeatOffset=0
        ;

    while(dataRepeat>rom.size){
      dataRepeat>>=1;
    }

    dataRepeatOffset=rom.size-dataRepeat;

    for(int j=0;j<dataRepeatAmount;j++){
      c.calc+=rom.data[(j%dataRepeatOffset)+dataRepeat];
    }
  }

  cc.calc=~c.calc;

  if(cc.header==cc.calc){
    printf("ROM checksum complement in header (0x%04x) matches calculated checksum complement (0x%04x)\n",cc.header,cc.calc);
  }else{
    printf("ROM checksum complement in header (0x%04x) does not match calculated checksum complemtnt (0x%04x)\n",cc.header,cc.calc);
    printf("Updating data\n");
    rom.data[headerBase+0xdc]=(cc.calc&0xff);
    rom.data[headerBase+0xdd]=(cc.calc>>8);
    rom.changed=1;
  }

  if(c.header==c.calc){
    printf("ROM checksum in header (0x%04x) matches calculated checksum (0x%04x)\n",c.header,c.calc);
  }else{
    printf("ROM checksum in header (0x%04x) does not match calculated checksum (0x%04x)\n",c.header,c.calc);
    printf("Updating data\n");
    rom.data[headerBase+0xde]=(c.calc&0xff);
    rom.data[headerBase+0xdf]=(c.calc>>8);
    rom.changed=1;
  }

}
