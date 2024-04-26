#ifndef CONTROLTHREAD_HPP
#define CONTROLTHREAD_HPP

#include <iostream>
#include <unistd.h>
#include <Python.h>

#include "SteeringBuffer.h"
#include "ThrottleBuffer.h"

class PiracerClass
{
    private:
        PyObject *pModule, *pClass, *pInstance;
        
    public:
        PiracerClass();
        ~PiracerClass();
        
        void applyThrottle(float throttle);
        void applySteering(float steering);
};

void *ControlThread(void *arg);

#endif
