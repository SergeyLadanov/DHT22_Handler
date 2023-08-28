#include "Model.hpp"
#include "DHT_Application.hpp"
#include "StorageApplication.hpp"
#include "TCP_ServerApplication.hpp"
#include "MQTT_Application.hpp"
#include "App.hpp"


float Model::GetTemperature(void)
{
    return App::GetDHT().GetTemperature();
}


float Model::GetHumidity(void)
{
    return App::GetDHT().GetHumidity();
}


void Model::StoreData(float temp, float hum)
{
    App::GetDataStorage().PutData(temp, hum);
}


char *Model::GetHumTopic(void)
{
    return App::GetMQTT().GetHumTopic();
}


char *Model::GetTempTopic(void)
{
    return App::GetMQTT().GetTempTopic();
}


char *Model::GetLwtTopic(void)
{
    return App::GetMQTT().GetLwtTopic();
}


char *Model::GetLwtOnlineMsg(void)
{
    return App::GetMQTT().GetOnlineMsg();
}


float Model::GetStoredHumByIndex(uint32_t index)
{
    return App::GetDataStorage().GetHumByIndex(index);
}


float Model::GetStoredTempByIndex(uint32_t index)
{
    return App::GetDataStorage().GetTempByIndex(index);
}


int Model::GetStoredHoursByIndex(uint32_t index)
{
    return App::GetDataStorage().GetHoursByIndex(index);
}


int Model::GetStoredMinutesByIndex(uint32_t index)
{
    return App::GetDataStorage().GetMinutesByIndex(index);
}


uint8_t Model::CheckStoredIndex(uint32_t index)
{
    return App::GetDataStorage().CheckIndex(index);
}


struct tm Model::GetTimeDate(void)
{
    return App::GetDataStorage().GetTimeDate();
}


uint32_t Model::GetStorageSize(void)
{
    return App::GetDataStorage().GetSize();
}