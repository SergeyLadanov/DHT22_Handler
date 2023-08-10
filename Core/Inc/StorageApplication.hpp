#ifndef __STORAGE_APPLICATION_HPP_
#define __STORAGE_APPLICATION_HPP_

#include <cstdint>
#include "datacontrol.h"
#include <pthread.h>

class StorageApplication
{
public:
    static void Init(void);
    static void PutData(float temp, float hum);

    static float GetHumByIndex(uint32_t index);
    static float GetTempByIndex(uint32_t index);
    static int GetHoursByIndex(uint32_t index);
    static int GetMinutesByIndex(uint32_t index);
    static uint8_t CheckIndex(uint32_t index);
    static struct tm GetTimeDate(void);
    static uint32_t GetSize(void);
private:
    static DC_StorageObj Hdc;
    static pthread_mutex_t Mutex;

};


#endif