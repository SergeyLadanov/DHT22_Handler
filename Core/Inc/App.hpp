#ifndef __APP_HPP_
#define __APP_HPP_

#include "DHT_Application.hpp"
#include "MQTT_Application.hpp"
#include "StorageApplication.hpp"
#include "TCP_ServerApplication.hpp"

class App
{
public:
    static inline DHT_Application &GetDHT()
    {
        return DHT_App;
    }

    static inline MQTT_Application &GetMQTT()
    {
        return MQTT_App;
    }

    static inline StorageApplication &GetDataStorage()
    {
        return DS_App;
    }

    static inline TCP_ServerApplication &GetTCPServer()
    {
        return TCP_App;
    }
    
private:
    static DHT_Application DHT_App;
    static MQTT_Application MQTT_App;
    static StorageApplication DS_App;
    static TCP_ServerApplication TCP_App;
};


#endif