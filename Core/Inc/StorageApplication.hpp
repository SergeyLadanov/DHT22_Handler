#ifndef __STORAGE_APPLICATION_HPP_
#define __STORAGE_APPLICATION_HPP_

#include <cstdint>
#include "datacontrol.h"
#include <pthread.h>

class StorageApplication
{
public:
    void Init(void);
    void PutData(float temp, float hum);

    float GetHumByIndex(uint32_t index);
    float GetTempByIndex(uint32_t index);
    int GetHoursByIndex(uint32_t index);
    int GetMinutesByIndex(uint32_t index);
    uint8_t CheckIndex(uint32_t index);
    struct tm GetTimeDate(void);
    uint32_t GetSize(void);
private:
    DC_StorageObj Hdc;
    pthread_mutex_t Mutex;

};


#endif