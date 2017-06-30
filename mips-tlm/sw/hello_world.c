#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int i;
  printf("Hi from processor MIPS!\n");

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

