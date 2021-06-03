#ifndef SEGA_8_PROCESSOR
  #define SEGA_8_PROCESSOR

  enum sega8{MASTER_SYSTEM,GAME_GEAR};

  class Sega8Bit:public Console{
    public:
      struct Size{
        unsigned int code=0
                     ,calc=0
                     ,actual=0
                     ;
      }size;

      //struct Checksum{
      //  unsigned int header=0
      //               ,actual=0
      //               ;
      //}checksum;

      struct Region{
        unsigned int code=0;
        std::string name="";
      }region;

      sega8 system;
      Checksum checksum;

      char segaHeader[9]
           ;

      //bool dataChanged=false;

      unsigned int reservedWord=0

        ,productCode=0
        ,version=0
        ;

      std::string console;

      //std::vector<unsigned char> data;

      Sega8Bit(char* fileName,sega8 inSystem){

        openFile(fileName,&data);

        system=inSystem;

        //Fancy short code to replace if/else
        system==MASTER_SYSTEM ? console="Master System/Mark III" : (system==GAME_GEAR ? console="Game Gear" : console="");

        std::cout<<"Processing SEGA "<<console<<" ROM: \""<<fileName<<"\"\n";

        size.actual=data.size();

        switch((size.actual)){
          case 32<<10:
          case 64<<10:
          case 128<<10:
          case 256<<10:
          case 512<<10:
            //std::cout<<"ROM size valid.\n";
            break;
          default:
            std::string errorMessage="Error. Invalid ROM size. Valid "+console+" ROM sizes are 32 Kb, 64 Kb, 128 Kb, 256 Kb, and 512 Kb.";
            throw std::runtime_error(errorMessage);
        }

        for(int i=0;i<8;i++){
          segaHeader[i]=data[0x7ff0+i];
        }

        segaHeader[8]=0;
        if(!strcmp(segaHeader,"TMR SEGA")==0){
          throw std::runtime_error("Invalid ROM header. Should be \"TMR SEGA\"");
        }

        reservedWord=data[0x7ff8]+(data[0x7ff9]<<8);
        checksum.header=data[0x7ffa]+(data[0x7ffb]<<8);
        productCode=data[0x7ffc]+(data[0x7ffd]<<8)+((data[0x7ffe]%16)<<16);
        version=(data[0x7ffe]>>4);
        region.code=(data[0x7fff]>>4);
        region.name=regions[region.code];

        size.code=(data[0x7fff]%16);
        size.calc=(sizes[size.code]<<10);

        std::cout<<"\nChecking for & correcting errors:\n";

        processSize();
        processRegion();
        processChecksum();

        if(!dataChanged){
          std::cout<<" - No errors found\n";
        }

        showInfo();

        std::cout<<"Internal data size: "<<(data.size())<<"\n";

        if(dataChanged){
          saveFile(fileName,&data);
        }
      }

    private:
      void processSize(){
        if(size.calc==0){
          unsigned int errorCode=size.code;

          for(int i=0;i<16;i++){
            if((sizes[i]<<10)==size.actual){
              size.code=i;
              break;
            }
          }
          std::cout<<std::hex<<" - Invalid ROM size in header ("<<errorCode<<"). Defaulting to actual ROM size ("<<size.code<<").\n"<<std::dec;
          dataChanged=true;
        }

        if(size.calc>size.actual){
          std::cout<<" - Defined ROM size is larger than actual ROM size. Defaulting to actual ROM size.\n";
          size.code=0xff;
          size.calc=size.actual;

          for(int i=0;i<16;i++){
            if((sizes[i]<<10)==size.actual){
              size.code=i;
              break;
            }
          }

          if(size.code>0x0f){
            std::string errorMessage="Invalid ROM size in header. Supported ROM sizes are 0: 256 Kb, 1: 512 Kb, c: 32 Kb, e: 64 Kb, f: 128 Kb";
            throw std::runtime_error(errorMessage);
          }
          dataChanged=true;
        }

        data[0x7fff]=(data[0x7fff]&0xf0)+size.code;

        size.calc=(sizes[size.code]<<10);
      }

      void processRegion(){
        if(region.name.compare("invalid")==0){
          std::cout<<" - Invalid region name. Defaulting to Japan.\n";
          system==MASTER_SYSTEM?region.code=3:region.code=5;
          region.name=regions[region.code];
          dataChanged=true;
          data[0x7fff]=(data[0x7fff]&0x0f)+(region.code<<4);
        }
      }

      void processChecksum(){
        calculateChecksum();

        if(checksum.header!=checksum.actual){
          std::cout<<std::hex<<" - Checksum in header ("<<checksum.header<<") does not match calculated checksum ("<<checksum.actual<<std::dec<<"). Correcting.\n";
          data[0x7ffa]=(checksum.actual%256);
          data[0x7ffb]=(checksum.actual>>8);
          checksum.header=data[0x7ffa]+(data[0x7ffb]<<8);
          dataChanged=true;
        }
      }

      void showInfo(){

        std::cout<<"\n";

        std::cout<<"SEGA header: "<<segaHeader<<"\n";
        std::cout<<"Reserved word: "<<std::hex<<reservedWord<<"\n";

        std::cout<<"Checksum: "<<std::hex<<checksum.header<<"\n";

        std::cout<<"Product code: "<<productCode<<"\n";
        std::cout<<"ROM version: "<<version<<"\n";


        std::cout<<"Region: "<<region.code<<" ("<<region.name<<")\n";

        std::cout<<"Cart size (in header): "<<size.code<<" ("<<std::dec<<size.calc<<" bytes, "<<std::dec<<(size.calc>>10)<<" Kb)\n";
        std::cout<<"Cart size (actual): "<<size.actual<<" ("<<std::dec<<(size.actual>>10)<<" Kb)\n";
      }

      void calculateChecksum(){
        unsigned int i=0;
        while(i<size.calc){
          checksum.actual+=data[i];
          i++;
          if(i==0x7ff0){
            i=0x8000;
          }
        }

        checksum.actual%=0x10000;
      }

      unsigned int sizes[16]={256,512,0,0,0,0,0,0,0,0,0,0,32,0,64,128};
      std::vector<std::string> regions={
          "invalid","invalid","invalid","SMS Japan","SMS Export","GG Japan"
          ,"GG Export","GG International","invalid","invalid","invalid","invalid"
          ,"invalid","invalid","invalid","invalid"};

  };
#endif