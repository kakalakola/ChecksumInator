#ifndef STRUCTS
#define STRUCTS

  typedef struct {
    unsigned int valid:1;
    unsigned int changed:1;
    long long unsigned int size;
    unsigned char* data;
  }ROM;

  typedef struct {
    unsigned int calc:8;
    unsigned int header:8;
  }checksum8;

  typedef struct {
    unsigned int calc:16;
    unsigned int header:16;
  }checksum16;

#endif