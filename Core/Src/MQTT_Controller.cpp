#include "MQTT_Controller.hpp"

#define UNUSED(X) ((void) X)


uint32_t MQTT_Controller::Delay = 0;

void MQTT_Controller::MQTT_OnConnected(MQTT_Client *obj)
{
    UNUSED(obj);
}


void MQTT_Controller::MQTT_OnDisconnected(MQTT_Client *obj)
{
    UNUSED(obj);
}



void MQTT_Controller::MQTT_OnReceived(MQTT_Client *obj, char *topic,  char *payload, uint16_t len)
{
    UNUSED(obj);
    UNUSED(topic);
    UNUSED(payload);
    UNUSED(len);
}



void MQTT_Controller::MQTT_PollConnection(MQTT_Client *obj)
{
        char Buf[32];

		Delay++;

		if (Delay >= 3)
		{
			printf("Sending...\r\n");

			if (strlen(ModelRef.GetTempTopic()))
			{
				snprintf(Buf, sizeof(Buf), "%2.3f", ModelRef.GetTemperature());
				obj->Publish(ModelRef.GetTempTopic(), 1, Buf);
			}

			if (strlen(ModelRef.GetHumTopic()))
			{
				snprintf(Buf, sizeof(Buf), "%2.3f", ModelRef.GetHumidity());
				obj->Publish(ModelRef.GetHumTopic(), 1, Buf);
			}

			Delay = 0;
		}
}