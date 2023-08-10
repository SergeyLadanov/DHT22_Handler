#ifndef __MODEL_HPP_
#define __MODEL_HPP_

#include <cstdint>
#include <time.h> 

class Model
{
public:
    float GetTemperature(void);
    float GetHumidity(void);
    void StoreData(float temp, float hum);
    void SendTextData(char *buf, uint32_t len);
    char *GetHumTopic(void);
    char *GetTempTopic(void);
    float GetStoredHumByIndex(uint32_t index);
    float GetStoredTempByIndex(uint32_t index);
    int GetStoredHoursByIndex(uint32_t index);
    int GetStoredMinutesByIndex(uint32_t index);
    uint8_t CheckStoredIndex(uint32_t index);
    struct tm GetTimeDate(void);
    uint32_t GetStorageSize(void);

};


#endif