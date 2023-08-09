#ifndef __DHT_APPLICATION_HPP_
#define __DHT_APPLICATION_HPP_

#include <pthread.h>

class DHT_Application
{
public:
    class IObserver
    {
    public:
        virtual void DHT_OnDataReady(void) = 0;
    };

public:
    static void Init(void);
    static float GetTemperature(void);
    static float GetHumidity(void);
    static void BindObserver(IObserver *observer);

private:
    static void* DHT_Task(void *args);
    static void Notify(void);

    static float Temperature;
    static float Humudity;
    static IObserver* Observer;
    static pthread_mutex_t Mutex;
    
};

#endif