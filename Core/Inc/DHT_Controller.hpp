#ifndef __DHT_CONTROLLER_HPP_
#define __DHT_CONTROLLER_HPP_

#include "DHT_Application.hpp"
#include "Model.hpp"

class DHT_Controller : public DHT_Application::IObserver
{
private:
    Model &ModelRef;
public:
    DHT_Controller(Model &model)
        : ModelRef(model)
    {

    }
private:

    void DHT_OnDataReady(void) override
    {

    }
};


#endif