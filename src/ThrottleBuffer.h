#ifndef THROTTLEBUFFER_H
#define THROTTLEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

extern float ThrottleBuffer;

extern pthread_mutex_t ThrottleBufferMutex;

#ifdef __cplusplus
}
#endif

#endif
