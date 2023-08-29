#ifndef __MQTT_APPLICATION_HPP_
#define __MQTT_APPLICATION_HPP_

#include <cstdint>
#include "MQTT_Client.hpp"

class MQTT_Application
{
public:
    MQTT_Application(const char *id = "none")
        :MQTT_Hanlder(id) 
    {

    }
    void Begin(const char *host, uint16_t port, const char *username, const char *password);
    void Begin(const char *host, uint16_t port, const char *username, const char *password, const char* willTopic, const char* willMessage);
    void SetHumTopic(char *topic);
    void SetTempTopic(char *topic);
    void Init(const char *id);
    void SetMsgOnline(char *msg);
    char *GetHumTopic(void);
    char *GetTempTopic(void);
    char *GetLwtTopic(void);
    char *GetOnlineMsg(void);
    char *GetOfflineMsg(void);
    void BindObserver(MQTT_Client::IObserver *observer);
private:
    struct Topics_t
    {
        char TempTopic[32];
        char HumTopic[32];
        char WillTopic[32];
    };

    MQTT_Client MQTT_Hanlder;
    Topics_t Topics;
    char WillOnlineMsg[32];
    char WillOfflineMsg[32];
};


#endif