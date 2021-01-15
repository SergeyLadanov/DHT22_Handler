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

//Thread defines
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

//DHT settings
#define MAX_TIMINGS 85
// Pin for sensor
#define DHT_PIN 7
// Length of data storage
#define DATA_LEN 24
// Listen port
#define SERVER_PORT 9090
// Length of tcp buffer
#define SEND_BUL_LEN 1025
// Period of collecting data (in minutes)
#define DATA_COLLECTING_PERIOD 30
// Размер медианного фильтра
#define MEDIAN_FILTER_SIZE 10



//Time data
typedef struct{
	int Hours;
	int Minutes;
}timeDataTypeDef;


//Storages for data
float hum_mass[DATA_LEN];
float temp_mass[DATA_LEN];
timeDataTypeDef time_mass[DATA_LEN];

uint8_t writeIndex = 0;
float temper = 0.0f;
float cur_temp = 0.0f;
float hum = 0.0f;
float cur_hum = 0.0f;
struct tm cur_time;
pthread_mutex_t mutex;
/********************MATH FUNCTIONS***************************/





DSP_MFN_Obj mfnHum, mfnTemp;
DSP_LPF1_Obj lpfHum, lpfTemp;



//-------------------------------------------------------------------
//Initialization of time hum_mass
void timeStorageIni(void)
{
	for (uint32_t i = 0; i < DATA_LEN; i++)
	{
		time_mass[i].Hours = -1;
		time_mass[i].Minutes = -1;
	}
}

/*************************DATA FUNCTIONS****************************/
//-----------------------------------------------------
float getLastTemp()
{
	return temp_mass[(writeIndex + (DATA_LEN - 1)) % DATA_LEN];
} 
//------------------------------------------------------
float getLastHum()
{
	return hum_mass[(writeIndex + (DATA_LEN - 1)) % DATA_LEN];
}
//------------------------------------------------------

void placeData(float h, float t, timeDataTypeDef *tm)
{
	hum_mass[writeIndex] = h;
	temp_mass[writeIndex] = t;
	time_mass[writeIndex].Hours = tm->Hours;
	time_mass[writeIndex].Minutes = tm->Minutes;
	writeIndex = (writeIndex + 1) % DATA_LEN;
}
//Module function
float module(float x)
{
	if (x < 0)
	{
		x = -x;
	}

	return x;
}


//Half hour detect function
uint8_t halfHour(struct tm *tim)
{
	static uint8_t flag = 0;

	if ((tim->tm_min % DATA_COLLECTING_PERIOD) == 0)
	{
		if (flag == 0)
		{
			flag = 1;
			return 1;
		}
	}
	else
	{
		flag = 0;
	}
	
	return 0;
}
//DHT task handler
void* dht_handle(void *args) {
	struct tm *u;
	time_t timer;
	timeDataTypeDef tm;
//	printf("Thread has been started!\n");

	DSP_MFN_Init(&mfnHum, MEDIAN_FILTER_SIZE);
	DSP_MFN_Init(&mfnTemp, MEDIAN_FILTER_SIZE);

	DSP_LPF1_Init(&lpfHum, 0.03, 0.02, 0.95);
	DSP_LPF1_Init(&lpfTemp, 0.03, 0.02, 0.95);

	while(1)
	{
		pthread_mutex_lock(&mutex);
		timer = time(NULL);
		u = localtime(&timer);
//		printf("%d/%d/%d %d:%d:%d\n", u->tm_mday, u->tm_mon + 1, u->tm_year % 100, u->tm_hour, u->tm_min, u->tm_sec);
		cur_time.tm_mday = u->tm_mday;
		cur_time.tm_mon = u->tm_mon;
		cur_time.tm_year = u->tm_year;
		cur_time.tm_hour = u->tm_hour;
		cur_time.tm_min = u->tm_min;
		cur_time.tm_sec = u->tm_sec;
		if (read_dht_data() == 1)
		{
			cur_temp = median_n(&hfTemp, temper);
			cur_hum = median_n(&hfHum, hum);

			cur_temp = rdspFilter(&lpfTemp, cur_temp);
			cur_hum = rdspFilter(&lpfHum, cur_hum);

//			printf( "By filter: %.1f;%.1f\n", cur_hum, cur_temp);
//			printf( "Without filter: %.1f;%.1f\n", hum, temper);
		}
		if (halfHour(u) == 1)
		{
			tm.Hours = u->tm_hour;
			tm.Minutes = u->tm_min;
//			printf("Putting data in storage...\n");
			placeData(hum, temper, &tm);
		}
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return SUCCESS;
}
//----------------------------------------------------------------
void repplyPrepare(char *buf, uint32_t len)
{
	char tmp[1024];
	uint32_t index;
	memset(buf, 0, len);
	sprintf(tmp, "%.1f;%.1f\r\n", cur_hum, cur_temp);
	strcat(buf, tmp);
	sprintf(tmp, "%02d:%02d %02d/%02d/%02d\r\n", cur_time.tm_hour, cur_time.tm_min, cur_time.tm_mday, cur_time.tm_mon + 1, cur_time.tm_year % 100);
	strcat(buf, tmp);
	// Put temperature mass
	for (uint32_t i = 0; i < DATA_LEN; i++)
	{
		index = (writeIndex + i) % DATA_LEN;
		if ((time_mass[index].Hours != -1) && (time_mass[index].Minutes != -1))
		{
			sprintf(tmp, "%.1f;",temp_mass[index]);
			strcat(buf, tmp);
		}
	}
	strcat(buf, "\r\n");
	// Put hum mass
	for (uint32_t i = 0; i < DATA_LEN; i++)
	{
		index = (writeIndex + i) % DATA_LEN;
		if ((time_mass[index].Hours != -1) && (time_mass[index].Minutes != -1))
		{
			sprintf(tmp, "%.1f;", hum_mass[index]);
			strcat(buf, tmp);
		}
	}
	strcat(buf, "\r\n");
	// Put hum mass
	for (uint32_t i = 0; i < DATA_LEN; i++)
	{
		index = (writeIndex + i) % DATA_LEN;
		if ((time_mass[index].Hours != -1) && (time_mass[index].Minutes != -1))
		{
			sprintf(tmp, "%02d:%02d;",time_mass[index].Hours, time_mass[index].Minutes);
			strcat(buf, tmp);
		}
	}
	
}


/***************************MAIN PROGRAM*************************/
int main(int argc, char *argv[]) {
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[SEND_BUL_LEN];

	pthread_t thread;
	int status;
	//Time mass ini
	timeStorageIni();
	//int status_addr;
	pthread_mutex_init(&mutex, NULL);
	status = pthread_create(&thread, NULL, dht_handle, NULL);

	if (status != 0) {
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);

	}

	if ( wiringPiSetup() == -1 )
	{
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
		// Accept a connection
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		// Preparing repply	
		pthread_mutex_lock(&mutex);		                                                                                                                                      
		repplyPrepare(sendBuff, sizeof(sendBuff));
		pthread_mutex_unlock(&mutex);
		// Send data
		write(connfd, sendBuff, strlen(sendBuff));
		// Closing connection
		close(connfd);
	}
}