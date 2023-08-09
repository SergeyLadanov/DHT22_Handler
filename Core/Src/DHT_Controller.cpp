#include "DHT_Controller.hpp"


void DHT_Controller::DHT_OnDataReady(void)
{
    ModelRef.StoreData(ModelRef.GetTemperature(), ModelRef.GetHumidity());
}