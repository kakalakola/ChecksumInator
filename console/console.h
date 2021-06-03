#ifndef CONSOLE_CLASS
  #define CONSOLE_CLASS
  class Console{
    public:
    protected:
      struct Checksum{
        unsigned int header=0
                     ,actual=0
                     ;
      };
      bool dataChanged=false;
      std::vector<unsigned char> data;
  };
#endif