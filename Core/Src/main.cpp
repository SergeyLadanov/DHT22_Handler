
#include "main.h"
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


	DHT_Application::Init();
	DHT_Application::BindObserver(&SensorController);
	StorageApplication::Init();
	TCP_ServerApplication::Init(SERVER_PORT);
	TCP_ServerApplication::BindObserver(&TCP_Listener);
	MQTT_Application::Init("DHT22_1");
	MQTT_Application::BindObserver(&MQTT_Handler);


	if (temp_topic && hum_topic)
	{
		MQTT_Application::SetHumTopic(hum_topic);
		MQTT_Application::SetTempTopic(temp_topic);
	}

	if (host && username && password && (port != 0))
	{
		MQTT_Application::Begin(host, port, username, password);
	}

	while(1) 
	{
		TCP_ServerApplication::Handle();
	}	
    
    return 0;  
}