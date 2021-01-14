#include "dht_if.h"


void DHT_PinMode(DHT_Obj *hdht, uint8_t mode)
{
    (void) mode;
}

//------------------------------------
void DHT_PinWrite(DHT_Obj *hdht, uint8_t state)
{
   (void) state;
}
//-------------------------------------
uint8_t DHT_PinRead(DHT_Obj *hdht)
{
    (void) NULL;
    return 0;
}
//----------------------------------------------
void DHT_Delay_us(DHT_Obj *hdht, uint32_t delay)
{
    (void) delay;
}
//----------------------------------------------
void DHT_Delay_ms(DHT_Obj *hdht, uint32_t delay)
{
    (void) delay;
}