#ifndef __MQTT_CONTROLLER_HPP__
#define __MQTT_CONTROLLER_HPP__

#include "MQTT_Client.hpp"
#include "Model.hpp"

class MQTT_Controller : public MQTT_Client::IObserver
{
private:
    Model &ModelRef;
public:
    MQTT_Controller(Model &model)
        : ModelRef(model)
    {

    }

private:
    void MQTT_OnConnected(MQTT_Client *obj) override;
    void MQTT_OnDisconnected(MQTT_Client *obj) override;
    void MQTT_OnReceived(MQTT_Client *obj, char *topic,  char *payload, uint16_t len) override;
    void MQTT_PollConnection(MQTT_Client *obj) override;


    uint32_t Delay = 0;
};

#endif