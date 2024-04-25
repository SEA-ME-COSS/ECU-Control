#include "ThrottleBuffer.h"

float ThrottleBuffer = 0.0f;

pthread_mutex_t ThrottleBufferMutex = PTHREAD_MUTEX_INITIALIZER;
