#ifndef __DHT_APPLICATION_HPP_
#define __DHT_APPLICATION_HPP_

#include <pthread.h>

class DHT_Application
{
public:
    class IObserver
    {
    public:
        virtual void DHT_OnDataReady(DHT_Application *obj) = 0;
    };

public:
    void Init(void);
    float GetTemperature(void);
    float GetHumidity(void);
    void BindObserver(IObserver *observer);

private:
    static void* DHT_Task(void *args);
    void Notify(void);

    float Temperature = 6.0;
    float Humudity = 35.0;
    IObserver* Observer = nullptr;
    pthread_mutex_t Mutex;
    
};

#endif