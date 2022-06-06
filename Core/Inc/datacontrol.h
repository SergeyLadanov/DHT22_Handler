#ifndef __DATACONTROL_H_
#define __DATACONTROL_H_

#ifdef __cplusplus
 extern "C" {
#endif

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
void DC_GetPacket(DC_StorageObj *hdc, char *buf, uint32_t len);

#ifdef __cplusplus
 }
#endif

#endif