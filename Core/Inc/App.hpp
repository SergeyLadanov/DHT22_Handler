#ifndef __APP_HPP_
#define __APP_HPP_

#include "DHT_Application.hpp"
#include "MQTT_Application.hpp"
#include "StorageApplication.hpp"

class App
{
public:
    static inline DHT_Application &DHT()
    {
        return DHT_App;
    }

    static inline MQTT_Application &MQTT()
    {
        return MQTT_App;
    }

    static inline StorageApplication &DataStorage()
    {
        return DS_App;
    }
private:
    static DHT_Application DHT_App;
    static MQTT_Application MQTT_App;
    static StorageApplication DS_App;
};


#endif