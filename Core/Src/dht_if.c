#include "dht.h"
#include <unistd.h>
#include "main.h"


#define DHT_PIN 7

void DHT_PinMode(DHT_Obj *hdht, uint8_t mode)
{
#if SIMULATOR == 0
    switch (mode)
    {
        case DHT_PIN_OUTPUT : 
            pinMode(DHT_PIN, OUTPUT);
        break;

        case DHT_PIN_INPUT :
            pinMode(DHT_PIN, INPUT);
        break;
    }
#endif
}

//------------------------------------
void DHT_PinWrite(DHT_Obj *hdht, uint8_t state)
{
#if SIMULATOR == 0
   digitalWrite(DHT_PIN, state);
#endif
}
//-------------------------------------
uint8_t DHT_PinRead(DHT_Obj *hdht)
{
#if SIMULATOR == 0
    return digitalRead(DHT_PIN);
#else
    return 0;
#endif
}
//----------------------------------------------
void DHT_Delay_us(DHT_Obj *hdht, uint32_t value)
{
#if SIMULATOR == 0
    delayMicroseconds(value);
#endif
}
//----------------------------------------------
void DHT_Delay_ms(DHT_Obj *hdht, uint32_t value)
{
#if SIMULATOR == 0
    delay(value);
#endif
}