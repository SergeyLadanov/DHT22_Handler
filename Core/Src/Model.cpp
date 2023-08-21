#include "Model.hpp"
#include "DHT_Application.hpp"
#include "StorageApplication.hpp"
#include "TCP_ServerApplication.hpp"
#include "MQTT_Application.hpp"


float Model::GetTemperature(void)
{
    return DHT_Application::GetTemperature();
}


float Model::GetHumidity(void)
{
    return DHT_Application::GetHumidity();
}


void Model::StoreData(float temp, float hum)
{
    StorageApplication::PutData(temp, hum);
}


void Model::SendTextData(char *buf, uint32_t len)
{
    TCP_ServerApplication::SendData((uint8_t *) buf, len);
}


char *Model::GetHumTopic(void)
{
    return MQTT_Application::GetHumTopic();
}


char *Model::GetTempTopic(void)
{
    return MQTT_Application::GetTempTopic();
}


char *Model::GetLwtTopic(void)
{
    return MQTT_Application::GetLwtTopic();
}


char *Model::GetLwtOnlineMsg(void)
{
    return MQTT_Application::GetOnlineMsg();
}


float Model::GetStoredHumByIndex(uint32_t index)
{
    return StorageApplication::GetHumByIndex(index);
}


float Model::GetStoredTempByIndex(uint32_t index)
{
    return StorageApplication::GetTempByIndex(index);
}


int Model::GetStoredHoursByIndex(uint32_t index)
{
    return StorageApplication::GetHoursByIndex(index);
}


int Model::GetStoredMinutesByIndex(uint32_t index)
{
    return StorageApplication::GetMinutesByIndex(index);
}


uint8_t Model::CheckStoredIndex(uint32_t index)
{
    return StorageApplication::CheckIndex(index);
}


struct tm Model::GetTimeDate(void)
{
    return StorageApplication::GetTimeDate();
}


uint32_t Model::GetStorageSize(void)
{
    return StorageApplication::GetSize();
}