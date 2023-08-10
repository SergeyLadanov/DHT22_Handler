#include "TCP_Controller.hpp"
#include <cstring>
#include <cstdio>


// Метод получения пакета со значениями
void TCP_Controller::GetPacket(char *buf, uint32_t len)
{
	char tmp[1024];
	uint32_t index;
	memset(buf, 0, len);
	sprintf(tmp, "%.1f;%.1f\r\n", ModelRef.GetHumidity(), ModelRef.GetTemperature());
	strcat(buf, tmp);
	sprintf(tmp, "%02d:%02d %02d/%02d/%02d\r\n", ModelRef.GetTimeDate().tm_hour, ModelRef.GetTimeDate().tm_min, ModelRef.GetTimeDate().tm_mday, ModelRef.GetTimeDate().tm_mon + 1, ModelRef.GetTimeDate().tm_year % 100);
	strcat(buf, tmp);
	// Put temperature mass
	for (uint32_t i = 0; i < ModelRef.GetStorageSize(); i++)
	{
		if (ModelRef.CheckStoredIndex(i))
		{
			sprintf(tmp, "%.1f;",ModelRef.GetStoredTempByIndex(i));
			strcat(buf, tmp);
		}
	}
	strcat(buf, "\r\n");
	// Put hum mass
	for (uint32_t i = 0; i < ModelRef.GetStorageSize(); i++)
	{

		if (ModelRef.CheckStoredIndex(i))
		{
			sprintf(tmp, "%.1f;", ModelRef.GetStoredHumByIndex(i));
			strcat(buf, tmp);
		}
	}
	strcat(buf, "\r\n");
	// Put hum mass
	for (uint32_t i = 0; i < ModelRef.GetStorageSize(); i++)
	{
		if (ModelRef.GetStoredHumByIndex(i))
		{
			sprintf(tmp, "%02d:%02d;", ModelRef.GetStoredHoursByIndex(i), ModelRef.GetStoredMinutesByIndex(i));
			strcat(buf, tmp);
		}
	}
	
}


void TCP_Controller::OnTcpConnected(void)
{
    GetPacket(SendBuf, sizeof(SendBuf));
    ModelRef.SendTextData(SendBuf, strlen(SendBuf));
}