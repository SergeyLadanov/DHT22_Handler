#ifndef __DATACONTROL_H_
#define __DATACONTROL_H_

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
    struct tm CurTime;
}DC_StorageObj;



#endif