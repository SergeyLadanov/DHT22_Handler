#include "DHT_Application.hpp"
#include "dsp_filters.h"
#include "dht_if.h"

// Определения для задач
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0



float DHT_Application::Temperature = 0.0f;
float DHT_Application::Humudity = 0.0f;
DHT_Application::IObserver* DHT_Application::Observer = nullptr;
pthread_mutex_t DHT_Application::Mutex;



void DHT_Application::Init(void)
{
    pthread_t thread;
    int status;
    pthread_mutex_init(&Mutex, NULL);
    status = pthread_create(&thread, NULL, DHT_Task, NULL);
}


float DHT_Application::GetTemperature(void)
{
    float res;
    pthread_mutex_lock(&Mutex);		                                                                                                                                      
    res = Temperature;
    pthread_mutex_unlock(&Mutex);
    return res;
}


float DHT_Application::GetHumidity(void)
{
    float res;
    pthread_mutex_lock(&Mutex);		                                                                                                                                      
    res = Humudity;
    pthread_mutex_unlock(&Mutex);
    return res;
}


void DHT_Application::BindObserver(IObserver *observer)
{
    Observer = observer;
}


void* DHT_Application::DHT_Task(void *args)
{
	struct tm *u;
	time_t timer;
	// Буфер медианного фильтра температуры
	// static float mfn_temp_buf[MEDIAN_FILTER_SIZE]={0};
	// // Буфер медианного фильтра влажности
	// static float mfn_hum_buf[MEDIAN_FILTER_SIZE]={0};

	//DSP_MFN_Obj mfnHum, mfnTemp;
	DSP_MF3_Obj mfnHum, mfnTemp;
	DSP_LPF1_Obj lpfHum, lpfTemp;
	DHT_Obj hdht;
	DHT_Result *dhtr;

	
	printf("Thread has been started!\n");

	// Инициализация медианных фильтров
	// DSP_MFN_Init(&mfnHum, mfn_hum_buf, MEDIAN_FILTER_SIZE);
	// DSP_MFN_Init(&mfnTemp, mfn_temp_buf, MEDIAN_FILTER_SIZE);

	

	// Инициализация фнч первого порядка
	DSP_LPF1_Init(&lpfHum, 0.03, 0.02, 0.95);
	DSP_LPF1_Init(&lpfTemp, 0.03, 0.02, 0.95);

	while(1)
	{
		dhtr = DHT_Read(&hdht);

		if (dhtr != NULL)
		{
            pthread_mutex_lock(&Mutex);
			printf("T = %.2f H = %.2f\r\n", dhtr->Temperature, dhtr->Humidity);
			Temperature = DSP_MF3_Handle(&mfnTemp, dhtr->Temperature);
			Humudity = DSP_MF3_Handle(&mfnHum, dhtr->Humidity);

			printf("After median filtering: T = %.2f H = %.2f\r\n", Temperature, Humudity);

			Temperature = DSP_LPF1_Handle(&lpfTemp, Temperature);
			Humudity = DSP_LPF1_Handle(&lpfHum, Humudity);

			printf("After lpf filtering: T = %.2f H = %.2f\r\n", Temperature, Humudity);

            pthread_mutex_unlock(&Mutex);
		}

        Notify();

		sleep(2);
	}
	return SUCCESS;
}

void DHT_Application::Notify(void)
{
    if (Observer)
    {
        Observer->DHT_OnDataReady();
    }
}