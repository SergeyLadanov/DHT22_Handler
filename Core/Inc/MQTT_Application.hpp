#ifndef __MQTT_APPLICATION_HPP_
#define __MQTT_APPLICATION_HPP_

#include <cstdint>
#include "MQTT_Client.hpp"

class MQTT_Application
{
public:
    static void Begin(const char *host, uint16_t port, const char *username, const char *password);
    static void SetHumTopic(char *topic);
    static void SetTempTopic(char *topic);
    static void Init(const char *id);
    static char *GetHumTopic(void);
    static char *GetTempTopic(void);
    static void BindObserver(MQTT_Client::IObserver *observer);
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