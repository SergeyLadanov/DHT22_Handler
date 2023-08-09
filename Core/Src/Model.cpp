#include "Model.hpp"
#include "DHT_Application.hpp"


float Model::GetTemperature(void)
{
    return DHT_Application::GetTemperature();
}


float Model::GetHumidity(void)
{
    return DHT_Application::GetHumidity();
}