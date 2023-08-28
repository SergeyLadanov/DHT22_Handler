
#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"

#ifndef SIMULATOR
#define SIMULATOR 0
#endif


#ifndef DHT_PIN
#define DHT_PIN 7
#endif


#if SIMULATOR == 0
#include <wiringPi.h>
#endif

#ifdef __cplusplus
}
#endif

#endif

