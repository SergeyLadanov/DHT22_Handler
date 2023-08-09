#include "Model.hpp"
#include "DHT_Application.hpp"
#include "StorageApplication.hpp"


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