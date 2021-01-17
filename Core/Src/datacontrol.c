#include "datacontrol.h"

// Проверка истечения периода записиЫ 
static uint8_t DC_PeriodElapsed(DC_StorageObj *hdc)
{
	if ((hdc->CurTime.tm_min % hdc->PeriodValue) == 0)
	{
		if (hdc->PeriodFlag == 0)
		{
			hdc->PeriodFlag = 1;
			return 1;
		}
	}
	else
	{
		hdc->PeriodFlag = 0;
	}
	
	return 0;
}

// Функция помещеная данных в хранилище
static void DC_SaveData(DC_StorageObj *hdc)
{
	hdc->Hum[hdc->WriteIndex] = hdc->CurHum;
	hdc->Temp[hdc->WriteIndex] = hdc->CurTemp;
	hdc->Time[hdc->WriteIndex].Hours = hdc->CurTime.tm_hour;
	hdc->Time[hdc->WriteIndex].Minutes = hdc->CurTime.tm_min;
	hdc->WriteIndex = (hdc->WriteIndex + 1) % hdc->Size;
}

// Инициализация массива времени
static void DC_TimeStorageInit(DC_StorageObj *hdc)
{
	for (uint32_t i = 0; i < hdc->Size; i++)
	{
		hdc->Time[i].Hours = -1;
		hdc->Time[i].Minutes = -1;
	}
}

// Функция инициализации объекта хранилища
uint8_t DC_Init(DC_StorageObj *hdc, uint32_t size, uint32_t period)
{
	hdc->Size = size;
	hdc->PeriodValue = period;

	hdc->Temp = (float *) malloc(sizeof(float) * hdc->Size);

	if (hdc->Temp == NULL)
	{
		return 1;
	}

	hdc->Hum = (float *) malloc(sizeof(float) * hdc->Size);

	if (hdc->Hum == NULL)
	{
		return 1;
	}

	hdc->Time = (DC_TimeObj *) malloc(sizeof(DC_TimeObj) * hdc->Size);

	if (hdc->Time == NULL)
	{
		return 1;
	}

	DC_TimeStorageInit(hdc);

	return 0;

}

// Обработка данных
void DC_Handle(DC_StorageObj *hdc, float t, float h, struct tm *tim)
{
	hdc->CurTime.tm_hour = tim->tm_hour;
	hdc->CurTime.tm_min = tim->tm_min;
	hdc->CurTime.tm_mday = tim->tm_mday;
	hdc->CurTime.tm_mon = tim->tm_mon;
	hdc->CurTime.tm_year = tim->tm_year;
	hdc->CurTemp = t;
	hdc->CurHum = h;

	if (DC_PeriodElapsed(hdc) == 1)
	{
		DC_SaveData(hdc);
	}
}


// Функция получения пакета со значениями
void DC_GetPacket(DC_StorageObj *hdc, char *buf, uint32_t len)
{
	char tmp[1024];
	uint32_t index;
	memset(buf, 0, len);
	sprintf(tmp, "%.1f;%.1f\r\n", hdc->CurHum, hdc->CurTemp);
	strcat(buf, tmp);
	sprintf(tmp, "%02d:%02d %02d/%02d/%02d\r\n", hdc->CurTime.tm_hour, hdc->CurTime.tm_min, hdc->CurTime.tm_mday, hdc->CurTime.tm_mon + 1, hdc->CurTime.tm_year % 100);
	strcat(buf, tmp);
	// Put temperature mass
	for (uint32_t i = 0; i < hdc->Size; i++)
	{
		index = (hdc->WriteIndex + i) % hdc->Size;
		if ((hdc->Time[index].Hours != -1) && (hdc->Time[index].Minutes != -1))
		{
			sprintf(tmp, "%.1f;",hdc->Temp[index]);
			strcat(buf, tmp);
		}
	}
	strcat(buf, "\r\n");
	// Put hum mass
	for (uint32_t i = 0; i < hdc->Size; i++)
	{
		index = (hdc->WriteIndex + i) % hdc->Size;
		if ((hdc->Time[index].Hours != -1) && (hdc->Time[index].Minutes != -1))
		{
			sprintf(tmp, "%.1f;", hdc->Temp[index]);
			strcat(buf, tmp);
		}
	}
	strcat(buf, "\r\n");
	// Put hum mass
	for (uint32_t i = 0; i < hdc->Size; i++)
	{
		index = (hdc->WriteIndex + i) % hdc->Size;
		if ((hdc->Time[index].Hours != -1) && (hdc->Time[index].Minutes != -1))
		{
			sprintf(tmp, "%02d:%02d;",hdc->Time[index].Hours, hdc->Time[index].Minutes);
			strcat(buf, tmp);
		}
	}
	
}

