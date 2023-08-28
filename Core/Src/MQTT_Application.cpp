#include "MQTT_Application.hpp"






void MQTT_Application::Begin(const char *host, uint16_t port, const char *username, const char *password)
{
    MQTT_Hanlder.Begin(host, port, username, password);
}


void MQTT_Application::Begin(const char *host, uint16_t port, const char *username, const char *password, const char* willTopic, const char* willMessage)
{
    snprintf(Topics.WillTopic, sizeof(Topics.WillTopic), willTopic);
    snprintf(WillOfflineMsg, sizeof(WillOfflineMsg), willMessage);
    
    MQTT_Hanlder.Begin(host, port, username, password, Topics.WillTopic, 0, 1, WillOfflineMsg);
}


void MQTT_Application::SetHumTopic(char *topic)
{
    snprintf(Topics.HumTopic, sizeof(Topics.HumTopic), topic);
}


void MQTT_Application::SetTempTopic(char *topic)
{
    snprintf(Topics.TempTopic, sizeof(Topics.TempTopic), topic);
}


void MQTT_Application::Init(const char *id)
{
    MQTT_Hanlder.SetId(id);
}


char *MQTT_Application::GetHumTopic(void)
{
    return Topics.HumTopic;
}


char *MQTT_Application::GetTempTopic(void)
{
    return Topics.TempTopic;
}


void MQTT_Application::BindObserver(MQTT_Client::IObserver *observer)
{
    MQTT_Hanlder.BindObserver(observer);
}


void MQTT_Application::SetMsgOnline(char *msg)
{
    snprintf(WillOnlineMsg, sizeof(WillOnlineMsg), msg);
}


char *MQTT_Application::GetLwtTopic(void)
{
    return Topics.WillTopic;
}


char *MQTT_Application::GetOnlineMsg(void)
{
    return WillOnlineMsg;
}


char *MQTT_Application::GetOfflineMsg(void)
{
    return WillOfflineMsg;
}  