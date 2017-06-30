#include <stdio.h>
#include <stdlib.h>


volatile char* peripheral = (char *) 67108872U; 


int main(int argc, char *argv[]){
    //escrita na variável lock
    *peripheral ="MC733";

    exit(0); // To avoid cross-compiler exit routine
    return 0; // Never executed, just for compatibility
}
