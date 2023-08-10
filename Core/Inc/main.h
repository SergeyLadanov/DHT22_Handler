
#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define SIMULATOR 0


#if SIMULATOR == 0
#include <wiringPi.h>
#endif

#ifdef __cplusplus
}
#endif

#endif

