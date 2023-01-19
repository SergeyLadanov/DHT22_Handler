#ifndef __DHT_PRESENTER_HPP_
#define __DHT_PRESENTER_HPP_

#include "MQTT_Client.hpp"
#include <pthread.h>

#define UNUSED(X) ((void) X)

class DHT_Presetner : public MQTT_Client::IObserver
{
private:
	pthread_mutex_t &Mutex;
    float &Temp;
    float &Hum;
    char TempTopic[32];
    char HumTopic[32];
	uint32_t Delay = 0;
public:
    DHT_Presetner(pthread_mutex_t &mut, float &temp, float &hum, const char * temp_topic = nullptr, const char *hum_topic = nullptr)
		: Mutex(mut), Temp(temp), Hum(hum)
    {
        memset(TempTopic, 0, sizeof(TempTopic));
        memset(HumTopic, 0, sizeof(HumTopic));


        if (temp_topic != nullptr)
        {
            snprintf(TempTopic, sizeof(TempTopic), temp_topic);
        }
        
        if (hum_topic != nullptr)
        {
            snprintf(HumTopic, sizeof(HumTopic), hum_topic);
        }

        
    }

	void InitTopics(const char * temp_topic, const char *hum_topic)
	{
		snprintf(TempTopic, sizeof(TempTopic), temp_topic);
		snprintf(HumTopic, sizeof(HumTopic), hum_topic);
	}

private:
    void MQTT_OnConnected(MQTT_Client *obj) override
    {
		UNUSED(obj);
    }

    void MQTT_OnDisconnected(MQTT_Client *obj) override
    {
		UNUSED(obj);
    }

    void MQTT_OnReceived(MQTT_Client *obj, char *topic,  char *payload, uint16_t len) override
    {
		UNUSED(obj);
		UNUSED(topic);
		UNUSED(payload);
		UNUSED(len);
    }

    void MQTT_PollConnection(MQTT_Client *obj) override
    {
        char Buf[32];

		Delay++;

		if (Delay >= 3)
		{
			printf("Sending...\r\n");
			pthread_mutex_lock(&Mutex);
			if (strlen(TempTopic))
			{
				snprintf(Buf, sizeof(Buf), "%2.3f", Temp);
				obj->Publish(TempTopic, 1, Buf);
			}

			if (strlen(HumTopic))
			{
				snprintf(Buf, sizeof(Buf), "%2.3f", Hum);
				obj->Publish(HumTopic, 1, Buf);
			}
			
			pthread_mutex_unlock(&Mutex);
			Delay = 0;
		}
    }
};

#endif