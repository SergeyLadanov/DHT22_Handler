#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <wiringPi.h>
#include <stdint.h>
#include <pthread.h>
#include "dsp_filters.h"
#include "dht_if.h"
#include "datacontrol.h"

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
pthread_mutex_t mutex;
// Объект хранилища данных
DC_StorageObj hdc;


// Задача обработки датчика
void* dht_handle(void *args) {
	struct tm *u;
	time_t timer;
	DC_TimeObj tm;
	float temperature = 0.0f;
	float humudity = 0.0f;
	DSP_MFN_Obj mfnHum, mfnTemp;
	DSP_LPF1_Obj lpfHum, lpfTemp;
	DHT_Obj hdht;
	DHT_Result *dhtr;

	
	printf("Thread has been started!\n");

	// Инициализация медианных фильтров
	if (DSP_MFN_Init(&mfnHum, MEDIAN_FILTER_SIZE) != 0)
	{
		printf("Error of initialization mfn1!\r\n");
	}
	
	if (DSP_MFN_Init(&mfnTemp, MEDIAN_FILTER_SIZE) != 0)
	{
		printf("Error of initialization mfn2!\r\n");
	}
	// Инициализация фнч первого порядка
	DSP_LPF1_Init(&lpfHum, 0.03, 0.02, 0.95);
	DSP_LPF1_Init(&lpfTemp, 0.03, 0.02, 0.95);

	while(1)
	{
		pthread_mutex_lock(&mutex);
		timer = time(NULL);
		u = localtime(&timer);
		printf("Time and date: %d/%d/%d %d:%d:%d\n", u->tm_mday, u->tm_mon + 1, u->tm_year % 100, u->tm_hour, u->tm_min, u->tm_sec);
		dhtr = DHT_Read(hdht);

		if (dhtr != NULL)
		{
			temperature = DSP_MFN_Handle(&mfnTemp, dhtr->Temperature);
			humudity = DSP_MFN_Handle(&mfnHum, dhtr->Humidity);

			temperature = DSP_LPF1_Handle(&lpfTemp, temperature);
			humudity = DSP_LPF1_Handle(&lpfHum, humudity);
		}
		DC_Handle(&hdc, temperature, humudity, u);
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return SUCCESS;
}



// Функция main
int main(int argc, char *argv[]) {
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[SEND_BUL_LEN];

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
	if ( wiringPiSetup() == -1 )
	{
		printf("Error of initialization wiringPi!\r\n");
		exit(1);
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
}