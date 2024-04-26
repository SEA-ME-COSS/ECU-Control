#include "ReadCANThread.h"
#include "ControlThread.hpp"

int main ()
{
    pthread_t readCANThread, controlThread;
    
    pthread_create(&readCANThread, NULL, ReadCANThread, NULL);
    pthread_create(&controlThread, NULL, ControlThread, NULL);

    pthread_join(readCANThread, NULL);
    pthread_join(controlThread, NULL);

    return 0;
}

