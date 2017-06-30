#include <stdio.h>
#include <stdlib.h>

volatile int *lock = (volatile int *) 536870912U;  
volatile char* peripheral = (char *) 536870912U + 4; 

void acquireLock() {
    while (*lock);
}

void releaseLock() {
    *lock = 0;
}

int main(int argc, char *argv[]){
    //escrita na variável lock
    *peripheral =(char *)"MC733\0";

    exit(0);
    return 0; // Never executed, just for compatibility
}
