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


void Model::GetTextData(char *buf, uint32_t len)
{
    StorageApplication::GetStrData(buf, len);
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