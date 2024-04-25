#ifndef CONTROLTHREAD_HPP
#define CONTROLTHREAD_HPP

#include <iostream>
#include <unistd.h>

#include "SteeringBuffer.h"
#include "ThrottleBuffer.h"

void *ControlThread(void *arg);

#endif
