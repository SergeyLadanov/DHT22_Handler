#include "Model.hpp"
#include "DHT_Application.hpp"
#include "StorageApplication.hpp"
#include "TCP_ServerApplication.hpp"
#include "MQTT_Application.hpp"
#include "App.hpp"


float Model::GetTemperature(void)
{
    return App::DHT().GetTemperature();
}


float Model::GetHumidity(void)
{
    return App::DHT().GetHumidity();
}


void Model::StoreData(float temp, float hum)
{
    App::DataStorage().PutData(temp, hum);
}


void Model::SendTextData(char *buf, uint32_t len)
{
    TCP_ServerApplication::SendData((uint8_t *) buf, len);
}


char *Model::GetHumTopic(void)
{
    return App::MQTT().GetHumTopic();
}


char *Model::GetTempTopic(void)
{
    return App::MQTT().GetTempTopic();
}


char *Model::GetLwtTopic(void)
{
    return App::MQTT().GetLwtTopic();
}


char *Model::GetLwtOnlineMsg(void)
{
    return App::MQTT().GetOnlineMsg();
}


float Model::GetStoredHumByIndex(uint32_t index)
{
    return App::DataStorage().GetHumByIndex(index);
}


float Model::GetStoredTempByIndex(uint32_t index)
{
    return App::DataStorage().GetTempByIndex(index);
}


int Model::GetStoredHoursByIndex(uint32_t index)
{
    return App::DataStorage().GetHoursByIndex(index);
}


int Model::GetStoredMinutesByIndex(uint32_t index)
{
    return App::DataStorage().GetMinutesByIndex(index);
}


uint8_t Model::CheckStoredIndex(uint32_t index)
{
    return App::DataStorage().CheckIndex(index);
}


struct tm Model::GetTimeDate(void)
{
    return App::DataStorage().GetTimeDate();
}


uint32_t Model::GetStorageSize(void)
{
    return App::DataStorage().GetSize();
}