
#include "main.hpp"
#include "MQTT_Client.hpp"
#include "MQTT_Publisher.hpp"


// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
// #include <wiringPi.h>
#include <stdint.h>
#include <pthread.h>
#include "dsp_filters.h"
#include "dht_if.h"
#include "datacontrol.h"
#include "DHT_Presenter.hpp"


// Определения для задач
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

// Размер хранилища данных
#define DATA_LEN 24
// Открытый TCP порт
#define SERVER_PORT 9090
// Длина буфера отправки
#define SEND_BUL_LEN 1025
// Период сбора данных (в минутах)
#define DATA_COLLECTING_PERIOD 30
// Размер медианного фильтра
#define MEDIAN_FILTER_SIZE 10

// Объект мьютекса
static pthread_mutex_t mutex;
// Объект хранилища данных
static DC_StorageObj hdc;

static float temperature = 0.0f;
static float humudity = 0.0f;

static MQTT_Client MQTT_Hanlder("DHT22_1");
static DHT_Presetner DHT_Listener(mutex, temperature, humudity);


// Задача обработки датчика
void* dht_handle(void *args) {
	struct tm *u;
	time_t timer;
	// Буфер медианного фильтра температуры
	static float mfn_temp_buf[MEDIAN_FILTER_SIZE]={0};
	// Буфер медианного фильтра влажности
	static float mfn_hum_buf[MEDIAN_FILTER_SIZE]={0};

	DSP_MFN_Obj mfnHum, mfnTemp;
	DSP_LPF1_Obj lpfHum, lpfTemp;
	DHT_Obj hdht;
	DHT_Result *dhtr;

	
	printf("Thread has been started!\n");

	// Инициализация медианных фильтров
	DSP_MFN_Init(&mfnHum, mfn_hum_buf, MEDIAN_FILTER_SIZE);
	DSP_MFN_Init(&mfnTemp, mfn_temp_buf, MEDIAN_FILTER_SIZE);

	// Инициализация фнч первого порядка
	DSP_LPF1_Init(&lpfHum, 0.03, 0.02, 0.95);
	DSP_LPF1_Init(&lpfTemp, 0.03, 0.02, 0.95);

	while(1)
	{
		pthread_mutex_lock(&mutex);
		timer = time(NULL);
		u = localtime(&timer);
		printf("Time and date: %d/%d/%d %d:%d:%d\n", u->tm_mday, u->tm_mon + 1, u->tm_year % 100, u->tm_hour, u->tm_min, u->tm_sec);
		dhtr = DHT_Read(&hdht);

		if (dhtr != NULL)
		{
			printf("T = %.2f H = %.2f\r\n", dhtr->Temperature, dhtr->Humidity);
			temperature = DSP_MFN_Handle(&mfnTemp, dhtr->Temperature);
			humudity = DSP_MFN_Handle(&mfnHum, dhtr->Humidity);

			printf("After median filtering: T = %.2f H = %.2f\r\n", temperature, humudity);

			temperature = DSP_LPF1_Handle(&lpfTemp, temperature);
			humudity = DSP_LPF1_Handle(&lpfHum, humudity);

			printf("After lpf filtering: T = %.2f H = %.2f\r\n", temperature, humudity);
		}
		DC_Handle(&hdc, temperature, humudity, u);
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return SUCCESS;
}



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


    int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[SEND_BUL_LEN];

	char *host = nullptr;
	int port = 0;
	char *username = nullptr;
	char *password = nullptr;

	char *temp_topic = nullptr;
	char *hum_topic = nullptr;

	pthread_t thread;
	int status;
	// Инициализация хранилища данных
	if (DC_Init(&hdc, DATA_LEN, DATA_COLLECTING_PERIOD) != 0)
	{
		printf("Error of initialization data storage!\r\n");
	}

	pthread_mutex_init(&mutex, NULL);
	status = pthread_create(&thread, NULL, dht_handle, NULL);

	if (status != 0) {
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);

	}
	// Инициализация портов ввода вывода
	// if ( wiringPiSetup() == -1 )
	// {
	// 	printf("Error of initialization wiringPi!\r\n");
	// 	exit(1);
	// }

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

	MQTT_Hanlder.BindObserver(&DHT_Listener);

	if (host && username && password && (port != 0))
	{
		MQTT_Hanlder.Begin(host, port, username, password);
	}

	if (temp_topic && hum_topic)
	{
		DHT_Listener.InitTopics(temp_topic, hum_topic);
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serv_addr.sin_port = htons(SERVER_PORT);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while(1) {
		// Ожидание подключения
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		// Подготовка ответа	
		pthread_mutex_lock(&mutex);		                                                                                                                                      
		DC_GetPacket(&hdc, sendBuff, sizeof(sendBuff));
		pthread_mutex_unlock(&mutex);
		// Отправка ответа
		write(connfd, sendBuff, strlen(sendBuff));
		// Закрытие соединение
		close(connfd);
	}
    
    return 0;  
}