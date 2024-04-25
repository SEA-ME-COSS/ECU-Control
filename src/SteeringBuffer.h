#ifndef STEERINGBUFFER_H
#define STEERINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

extern float SteeringBuffer;

extern pthread_mutex_t SteeringBufferMutex;

#ifdef __cplusplus
}
#endif

#endif
