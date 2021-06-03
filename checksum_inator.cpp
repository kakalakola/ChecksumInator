#include "ci_io_handler.h"

int main(int argc,char* argv[]){
  std::cout<<COMMON_HEADER;

  if(argc==1){ //Check if no arguments are provided
    printUsage(argv[0]);
  }else if(argc==2){ //Check if only one argument is provided
    checkHelp(argv[0],argv[1]);
    std::cout<<"Unknown command: "<<argv[1]<<"\n";
    printUsage(argv[0]);
  }else if(argc>=3){ //Otherwise see if the provided arguments are valid
    checkHelp(argv[0],argv[1]);

    std::cout<<"\n";

    if(checkStrings(argv[1],&segagen)){
      std::cout<<"Processing Genesis/Mega Drive ROM: "<<argv[2]<<"\n";
    }else if((strcmp(argv[1],"32x")==0)){
      std::cout<<"Processing 32X ROM: "<<argv[2]<<"\n";
    }else if(checkStrings(argv[1],&snes)){
      std::cout<<"Processing Super NES/Super Famicom ROM: "<<argv[2]<<"\n";
    }else if((strcmp(argv[1],"gb")==0)){
      std::cout<<"Processing Game Boy ROM: "<<argv[2]<<"\n";
    }else if((strcmp(argv[1],"gbc")==0)){
      std::cout<<"Processing Game Boy Color ROM: "<<argv[2]<<"\n";
    }else if((strcmp(argv[1],"sgb")==0)){
      std::cout<<"Processing Super Game Boy ROM: "<<argv[2]<<"\n";
    }else if((strcmp(argv[1],"gba")==0)){
      GBA gba(argv[2]);
    }else if((strcmp(argv[1],"gg")==0)){
      Sega8Bit segaGG(argv[2],GAME_GEAR);
    }else if(checkStrings(argv[1],&segams)){
      Sega8Bit segaMS(argv[2],MASTER_SYSTEM);
    }else{
      std::cout<<"Unknown console: "<<argv[1]<<"\n\n";
      printUsage(argv[0]);
    }
  }else{
    //throw std::runtime_error("Invalid number of arguments\n");
    std::cout<<"Invalid number of arguments\n";
    printUsage(argv[0]);
  }

  return 0;
}
