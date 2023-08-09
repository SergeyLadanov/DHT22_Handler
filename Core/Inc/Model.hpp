#ifndef __MODEL_HPP_
#define __MODEL_HPP_

#include <cstdint>

class Model
{
public:
    float GetTemperature(void);
    float GetHumidity(void);
    void StoreData(float temp, float hum);
    void GetTextData(char *buf, uint32_t len);

};


#endif