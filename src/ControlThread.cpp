#include "ControlThread.hpp"

void *ControlThread(void *arg)
{
    while (1)
    {
        // Read steering data from steering buffer
        pthread_mutex_lock(&SteeringBufferMutex);
        std::cout<<"steering"<<std::endl;
        pthread_mutex_unlock(&SteeringBufferMutex);
        
        // Read throttle data from throttle buffer
        pthread_mutex_lock(&ThrottleBufferMutex);
        std::cout<<"throttle"<<std::endl;
        pthread_mutex_unlock(&ThrottleBufferMutex);
        
        usleep(3000000);  // Sleep for 300 ms
    }
    
    return NULL;
}
