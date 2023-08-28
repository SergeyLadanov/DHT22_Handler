
#include "main.h"
#include "App.hpp"
#include "MQTT_Client.hpp"

#include "DHT_Application.hpp"
#include "StorageApplication.hpp"
#include "TCP_ServerApplication.hpp"
#include "DHT_Controller.hpp"
#include "TCP_Controller.hpp"
#include "Model.hpp"
#include "MQTT_Application.hpp"
#include "MQTT_Controller.hpp"




#define SERVER_PORT 9090



static Model DataModel;
static DHT_Controller SensorController(DataModel);
static TCP_Controller TCP_Listener(DataModel);
static MQTT_Controller MQTT_Handler(DataModel);


// Основная программа
int main(int argc, char *argv[])
{
    
    #if defined(_WIN32) || defined(_WIN64)//Windows includes
    int err = 0;
    WSADATA wsaData;
    err = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (err != 0)
	{
		printf("Errore in WSAStartup\n");
        exit(-1);
	} 
    #endif


	char *host = nullptr;
	int port = 0;
	char *username = nullptr;
	char *password = nullptr;

	char *temp_topic = nullptr;
	char *hum_topic = nullptr;

	char *lwt_topic = nullptr;
	char *lwt_online_msg = nullptr;
	char *lwt_offline_msg = nullptr;



	// Инициализация портов ввода вывода
#if SIMULATOR == 0
	if ( wiringPiSetup() == -1 )
	{
		printf("Error of initialization wiringPi!\r\n");
		exit(1);
	}
#endif




	if (argc > 1)
    {
        host = argv[1];
    }
        
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }
        
    if (argc > 3)
    {
        username = argv[3];
    }
        
    if (argc > 4)
    {
        password = argv[4];
    }

	if (argc > 5)
    {
        temp_topic = argv[5];
    }
        
    if (argc > 6)
    {
        hum_topic = argv[6];
    }

	if (argc > 7)
    {
        lwt_topic = argv[7];
    }

	if (argc > 8)
    {
        lwt_online_msg = argv[8];
    }

	if (argc > 9)
    {
        lwt_offline_msg = argv[9];
    }


	App::DHT().Init();
	App::DHT().BindObserver(&SensorController);
	App::DataStorage().Init();
	TCP_ServerApplication::Init(SERVER_PORT);
	TCP_ServerApplication::BindObserver(&TCP_Listener);
	App::MQTT().Init("DHT22_1");
	App::MQTT().BindObserver(&MQTT_Handler);


	if (temp_topic && hum_topic)
	{
		App::MQTT().SetHumTopic(hum_topic);
		App::MQTT().SetTempTopic(temp_topic);
	}

	if (host && username && password && (port != 0))
	{
		if (lwt_topic && lwt_online_msg && lwt_offline_msg)
		{
			App::MQTT().SetMsgOnline(lwt_online_msg);
			App::MQTT().Begin(host, port, username, password, lwt_topic, lwt_offline_msg);
		}
		else
		{
			App::MQTT().Begin(host, port, username, password);
		}
		
	}

	while(1) 
	{
		TCP_ServerApplication::Handle();
	}	
    
    return 0;  
}