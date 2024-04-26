#include "ControlThread.hpp"

PiracerClass::PiracerClass()
{
    Py_Initialize();
    pModule = PyImport_ImportModule("piracer.vehicles");
    pClass = PyObject_GetAttrString(pModule, "PiRacerStandard");
    pInstance = PyObject_CallObject(pClass, NULL);
}

PiracerClass::~PiracerClass()
{
    Py_DECREF(pInstance);
    Py_DECREF(pClass);
    Py_DECREF(pModule);
    Py_Finalize();
}

void PiracerClass::applySteering(float steering)
{
    PyObject_CallMethod(pInstance, "set_steering_percent", "(f)", steering * -1.0);

    return;
}

void PiracerClass::applyThrottle(float throttle)
{
    PyObject_CallMethod(pInstance, "set_throttle_percent", "(f)", throttle * 0.5);

    return;
}

void *ControlThread(void *arg)
{
    PiracerClass piracer;
    
    while (1)
    {
        // Read steering data from steering buffer
        pthread_mutex_lock(&SteeringBufferMutex);
        std::cout<<"steering: "<<SteeringBuffer<<std::endl;
        pthread_mutex_unlock(&SteeringBufferMutex);
        
        // Read throttle data from throttle buffer
        pthread_mutex_lock(&ThrottleBufferMutex);
        std::cout<<"throttle: "<<ThrottleBuffer<<std::endl;
        pthread_mutex_unlock(&ThrottleBufferMutex);
        
        usleep(1000000);  // Sleep for 300 ms
    }
    
    return NULL;
}













