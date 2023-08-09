#ifndef __MQTT_APPLICATION_HPP_
#define __MQTT_APPLICATION_HPP_

#include <cstdint>
#include "MQTT_Client.hpp"

class MQTT_Application
{
public:
    
private:
    struct Topics_t
    {
        char TempTopic[32];
        char HumTopic[32];
    };

    static MQTT_Client MQTT_Hanlder;
    static Topics_t Topics;
};


#endif