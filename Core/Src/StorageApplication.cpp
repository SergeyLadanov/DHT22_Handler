#include "StorageApplication.hpp"
#include <time.h> 



// Размер хранилища данных
#define DATA_LEN 24

// Период сбора данных (в минутах)
#define DATA_COLLECTING_PERIOD 30



DC_StorageObj StorageApplication::Hdc;
pthread_mutex_t StorageApplication::Mutex;


void StorageApplication::Init(void)
{
	if (DC_Init(&Hdc, DATA_LEN, DATA_COLLECTING_PERIOD) != 0)
	{
		printf("Error of initialization data storage!\r\n");
	}

    pthread_mutex_init(&Mutex, NULL);
}


void StorageApplication::PutData(float temp, float hum)
{
    struct tm *u;
	time_t timer;
    timer = time(NULL);
    u = localtime(&timer);
	printf("Time and date: %d/%d/%d %d:%d:%d\n", u->tm_mday, u->tm_mon + 1, u->tm_year % 100, u->tm_hour, u->tm_min, u->tm_sec);

    pthread_mutex_lock(&Mutex);
    DC_Handle(&Hdc, temp, hum, u);
    pthread_mutex_unlock(&Mutex);	
}


void StorageApplication::GetStrData(char *buf, uint32_t len)
{
    pthread_mutex_lock(&Mutex);
    DC_GetPacket(&Hdc, buf, len);
    pthread_mutex_unlock(&Mutex);
}



float StorageApplication::GetHumByIndex(uint32_t index)
{
    float res;
    pthread_mutex_lock(&Mutex);
    res = DC_GetHumByIndex(&Hdc, index);
    pthread_mutex_unlock(&Mutex);
    return res;
}


float StorageApplication::GetTempByIndex(uint32_t index)
{
    float res;
    pthread_mutex_lock(&Mutex);
    res = DC_GetTempByIndex(&Hdc, index);
    pthread_mutex_unlock(&Mutex);
    return res;
}


int StorageApplication::GetHoursByIndex(uint32_t index)
{
    int res;
    pthread_mutex_lock(&Mutex);
    res = DC_GetHoursByIndex(&Hdc, index);
    pthread_mutex_unlock(&Mutex);
    return res;
}


int StorageApplication::GetMinutesByIndex(uint32_t index)
{
    int res;
    pthread_mutex_lock(&Mutex);
    res = DC_GetMinutesByIndex(&Hdc, index);
    pthread_mutex_unlock(&Mutex);
    return res;
}


uint8_t StorageApplication::CheckIndex(uint32_t index)
{
    uint8_t res;
    pthread_mutex_lock(&Mutex);
    res = DC_CheckIndex(&Hdc, index);
    pthread_mutex_unlock(&Mutex);
    return res;
}


struct tm StorageApplication::GetTimeDate(void)
{
    struct tm res;
    pthread_mutex_lock(&Mutex);
    res = DC_GetTimeDate(&Hdc);
    pthread_mutex_unlock(&Mutex);
    return res;
}