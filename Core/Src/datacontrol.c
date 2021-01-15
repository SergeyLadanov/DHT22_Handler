#include "datacontrol.h"

// Проверка 
uint8_t DC_PeriodElapsed(DC_StorageObj *hdc, struct tm *tim)
{
	if ((tim->tm_min % hdc->PeriodValue) == 0)
	{
		if (hdc->PeriodFlag == 0)
		{
			hdc->PeriodFlag;
			return 1;
		}
	}
	else
	{
		hdc->PeriodFlag = 0;
	}
	
	return 0;
}


void DC_GetPacket(DC_StorageObj *hdc, char *buf, uint32_t len)
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
		if ((hdc->Time[index].Hours != -1) && (hdc->Time[index].Minutes != -1))
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
		if ((hdc->Time[index].Hours != -1) && (hdc->Time[index].Minutes != -1))
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
		if ((hdc->Time[index].Hours != -1) && (hdc->Time[index].Minutes != -1))
		{
			sprintf(tmp, "%02d:%02d;",hdc->Time[index].Hours, hdc->Time[index].Minutes);
			strcat(buf, tmp);
		}
	}
	
}

