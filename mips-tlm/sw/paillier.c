#include <stdio.h>
#include <stdlib.h>

volatile int *lock = (volatile int *) 67108872U;
volatile char* peripheral = (char *) 67108872U + 4; 

void acquireLock() {
    while (*lock);
}

void releaseLock() {
    *lock = 0;
}

int main(int argc, char *argv[]){
    //escrita na variável lock
    *peripheral =(char *)"MC733";
    
    acquireLock();

    exit(0);
    return 0; // Never executed, just for compatibility
}
