#include "DHT_Controller.hpp"


void DHT_Controller::DHT_OnDataReady(DHT_Application *obj)
{
    ModelRef.StoreData(obj->GetTemperature(), obj->GetHumidity());
}