#ifndef __MQTT_APPLICATION_HPP_
#define __MQTT_APPLICATION_HPP_

#include <cstdint>
#include "MQTT_Client.hpp"

class MQTT_Application
{
public:
    static void Begin(const char *host, uint16_t port, const char *username, const char *password);
    static void Begin(const char *host, uint16_t port, const char *username, const char *password, const char* willTopic, const char* willMessage);
    static void SetHumTopic(char *topic);
    static void SetTempTopic(char *topic);
    static void Init(const char *id);
    static void SetMsgOnline(char *msg);
    static void SetMsgOffline(char *msg);
    static char *GetHumTopic(void);
    static char *GetTempTopic(void);
    static char *GetLwtTopic(void);
    static char *GetOnlineMsg(void);
    static char *GetOfflineMsg(void);
    static void BindObserver(MQTT_Client::IObserver *observer);
private:
    struct Topics_t
    {
        char TempTopic[32];
        char HumTopic[32];
        char WillTopic[32];
    };

    static MQTT_Client MQTT_Hanlder;
    static Topics_t Topics;
    static char WillOnlineMsg[32];
    static char WillOfflineMsg[32];
};


#endif