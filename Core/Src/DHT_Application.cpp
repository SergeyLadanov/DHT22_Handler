#include "DHT_Application.hpp"
#include "dsp_filters.h"
#include "dht.h"

// Определения для задач
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

#define USE_KALMAN_FILTER 1



void DHT_Application::Init(void)
{
    pthread_t thread;
    int status;

    pthread_mutex_init(&Mutex, NULL);
    status = pthread_create(&thread, NULL, DHT_Task, this);


	if (status != 0) 
	{
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);
	}
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
	DHT_Application *obj = (DHT_Application *) args;
	struct tm *u;
	time_t timer;

	DSP_MF3_Obj mfnHum, mfnTemp;
	
	DHT_Obj hdht;
	DHT_Result *dhtr;

#if USE_KALMAN_FILTER != 0
	DSP_Kalman_Obj hKalmanHum, hKalmanTemp;
	DSP_Kalman_Init(&hKalmanHum, 1.0f, 0.001);
	DSP_Kalman_Init(&hKalmanTemp, 1.0f, 0.001);
#else	
	DSP_LPF1_Obj lpfHum, lpfTemp;
	// Инициализация фнч первого порядка
	DSP_LPF1_Init(&lpfHum, 0.03, 0.02, 0.95);
	DSP_LPF1_Init(&lpfTemp, 0.03, 0.02, 0.95);
#endif


	printf("DHT thread has been started!\n");

	while(1)
	{
		dhtr = DHT_Read(&hdht);

		if (dhtr != NULL)
		{
            pthread_mutex_lock(&obj->Mutex);
			printf("T = %.2f H = %.2f\r\n", dhtr->Temperature, dhtr->Humidity);
			obj->Temperature = DSP_MF3_Handle(&mfnTemp, dhtr->Temperature);
			obj->Humudity = DSP_MF3_Handle(&mfnHum, dhtr->Humidity);

			printf("After median filtering: T = %.2f H = %.2f\r\n", obj->Temperature, obj->Humudity);

#if USE_KALMAN_FILTER != 0
			obj->Temperature = DSP_Kalman_Handle(&hKalmanTemp, obj->Temperature);
			obj->Humudity = DSP_Kalman_Handle(&hKalmanHum,obj->Humudity);
#else
			Temperature = DSP_LPF1_Handle(&lpfTemp, Temperature);
			Humudity = DSP_LPF1_Handle(&lpfHum, Humudity);
#endif

			printf("After lpf filtering: T = %.2f H = %.2f\r\n", obj->Temperature, obj->Humudity);

            pthread_mutex_unlock(&obj->Mutex);
		}

        obj->Notify();

		sleep(2);
	}
	return SUCCESS;
}

void DHT_Application::Notify(void)
{
    if (Observer)
    {
        Observer->DHT_OnDataReady(this);
    }
}