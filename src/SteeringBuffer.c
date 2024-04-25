#include "SteeringBuffer.h"

float SteeringBuffer = 0.0f;

pthread_mutex_t SteeringBufferMutex = PTHREAD_MUTEX_INITIALIZER;
