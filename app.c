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
// Median filter Length
#define FILER_LENGTH 10


//Time data
typedef struct{
	int Hours;
	int Minutes;
}timeDataTypeDef;

uint8_t data[5] = { 0, 0, 0, 0, 0 };

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
float mod(float x)
{
	if (x < 0.0f)
	{
		x = -x;
	}

	return x;
}
//------------------------------------------------

// Обработчик фильтра первого порядка
typedef struct{
    float X1; // Предыдущее значение входного сигнала
    float Y1; // Предыдущее значение фильтра
    float KX0; // Коэффициент текущего значения входного сигнала
    float KX1; // Коэффициент предыдущего значения входного сигнала
    float KY1; // Коэффициент предыдущего значения выходного сигнала
}hFilterTypeDef;

//Median filter
typedef struct{
	float Buf[FILER_LENGTH];
	uint8_t Cnt;
}HandlerMedianFilter;

HandlerMedianFilter hfHum, hfTemp;
hFilterTypeDef lpfHum, lpfTemp;

//---------------------------------------------------------------
void rdspFilterInit(hFilterTypeDef *hFilter, float kx0, float kx1, float ky1)
{
    hFilter->KX0 = kx0;
    hFilter->KX1 = kx1;
    hFilter->KY1 = ky1;
}
//-------------------------------------------------------------------

float rdspFilter(hFilterTypeDef *hFilter, float X0)
{
    float Y0 = 0.0f;

    Y0 = hFilter->KX0 * X0 + hFilter->KX1 * hFilter->X1 + hFilter->KY1 * hFilter->Y1;
    hFilter->Y1 = Y0;
    hFilter->X1 = X0;

    return Y0;
}

// Median filter function for N values
float median_n(HandlerMedianFilter *hFilter, float newVal)
{
	float *buffer = hFilter->Buf;
	buffer[hFilter->Cnt] = newVal;
	if ((hFilter->Cnt < FILER_LENGTH - 1) && (buffer[hFilter->Cnt] > buffer[hFilter->Cnt + 1])) 
	{
		for (int i = hFilter->Cnt; i < FILER_LENGTH - 1; i++) 
		{
			if (buffer[i] > buffer[i + 1]) 
			{
				float buff = buffer[i];
				buffer[i] = buffer[i + 1];
				buffer[i + 1] = buff;
			}
		}
	}
	else 
	{
		if ((hFilter->Cnt > 0) && (buffer[hFilter->Cnt - 1] > buffer[hFilter->Cnt])) 
		{
			for (int i = hFilter->Cnt; i > 0; i--) 
			{
				if (buffer[i] < buffer[i - 1])
				{
					float buff = buffer[i];
					buffer[i] = buffer[i - 1];
					buffer[i - 1] = buff;
				}
			}
		}
	}
	hFilter->Cnt = (hFilter->Cnt + 1) % FILER_LENGTH;
	return buffer[(int)FILER_LENGTH / 2];
}
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
// Get dht sensor data
uint8_t read_dht_data()
{
    uint8_t j = 0, i;
	uint32_t cnt_delay = 0xFFFFFFFF;

// Variables for protect for wrong values
//	static float temp_1 = 0.0f; // Backup value of temperature
//	static float hum_1 = 0.0f; // Backup value of hum
//	static uint8_t tries = 0; // Tries counter
//------------------------------------------

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    /* pull pin down for 18 milliseconds */
    pinMode( DHT_PIN, OUTPUT );
    digitalWrite( DHT_PIN, LOW );
    delay( 18 );
    /* prepare to read the pin */
    pinMode( DHT_PIN, INPUT );
    delayMicroseconds(39);
    if (digitalRead(DHT_PIN) == 1)
    {
		return 0;
    }

    delayMicroseconds(80);

    if (digitalRead(DHT_PIN) == 0)
    {
		return 0;
    }

    delayMicroseconds(80);

    for (j = 0; j < 5; j++)
    {
		data[4-j]=0;
		for(i=0; i<8; i++)
		{
			cnt_delay = 0xFFFFFFFF;
			while(digitalRead(DHT_PIN) == 0)
			{
				cnt_delay--;
				if (cnt_delay == 0)
				{
					return 0;
				}
			}
			delayMicroseconds(30);
			if (digitalRead(DHT_PIN) == 1)
			{
				data[4-j] |= (1<<(7-i));
			}
			cnt_delay = 0xFFFFFFFF;
			while(digitalRead(DHT_PIN) == 1)
			{
				cnt_delay--;
				if (cnt_delay == 0)
				{
					return 0;
				}
			}
		}
    }

    temper = (float)((*(uint16_t*)(data+1)) & 0x3FFF) / 10;
    if((*(uint16_t*)(data+1)) & 0x8000) temper *= -1.0;

    hum = (float)(*(int16_t*)(data+3)) / 10;

    return 1;
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

	rdspFilterInit(&lpfHum, 0.03, 0.02, 0.95);
	rdspFilterInit(&lpfTemp, 0.03, 0.02, 0.95);

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