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
    static void GetStrData(char *buf, uint32_t len);
private:
    static DC_StorageObj Hdc;
    static pthread_mutex_t Mutex;

};


#endif