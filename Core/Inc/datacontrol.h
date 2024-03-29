#ifndef __DATACONTROL_H_
#define __DATACONTROL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <stdint.h>

// Объект времени
typedef struct{
	int Hours;
	int Minutes;
}DC_TimeObj;
// Объект хранилища данных
typedef struct{
    uint32_t PeriodValue;
	uint8_t PeriodFlag;
    uint32_t Size;
    float *Hum;
    float *Temp;
    DC_TimeObj *Time;
    float CurHum;
    float CurTemp;
    struct tm CurTime;
    uint32_t WriteIndex;
}DC_StorageObj;


uint8_t DC_Init(DC_StorageObj *hdc, uint32_t size, uint32_t period);
void DC_Handle(DC_StorageObj *hdc, float t, float h, struct tm *tim);
float DC_GetHumByIndex(DC_StorageObj *hdc, uint32_t index);
float DC_GetTempByIndex(DC_StorageObj *hdc, uint32_t index);
int DC_GetHoursByIndex(DC_StorageObj *hdc, uint32_t index);
int DC_GetMinutesByIndex(DC_StorageObj *hdc, uint32_t index);
uint8_t DC_CheckIndex(DC_StorageObj *hdc, uint32_t index);
struct tm DC_GetTimeDate(DC_StorageObj *hdc);
uint32_t DC_GetSize(DC_StorageObj *hdc);

#ifdef __cplusplus
 }
#endif

#endif