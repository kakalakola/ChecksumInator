#ifndef STRUCTS
#define STRUCTS

  struct romStat{
    unsigned int valid:1;
    unsigned int changed:1;
  };

  struct checksum8{
    unsigned int calc:8;
    unsigned int header:8;
  };
  struct checksum16{
    unsigned int calc:16;
    unsigned int header:16;
  };

#endif