#ifndef __DSP_FILTERS_H_
#define __DSP_FILTERS_H_

#include <stdint.h>
#include <stdlib.h>


// Обработчик медианного фильтра
typedef struct{
	float *Buf;
	uint8_t Cnt;
    uint32_t Size;
}DSP_MFN_Obj;


// Обработчик фильтра первого порядка
typedef struct{
    float X1; // Предыдущее значение входного сигнала
    float Y1; // Предыдущее значение фильтра
    float KX0; // Коэффициент текущего значения входного сигнала
    float KX1; // Коэффициент предыдущего значения входного сигнала
    float KY1; // Коэффициент предыдущего значения выходного сигнала
}DSP_LPF1_Obj;


void DSP_LPF1_Init(DSP_LPF1_Obj *hFilter, float kx0, float kx1, float ky1);
float DSP_LPF1_Handle(DSP_LPF1_Obj *hFilter, float X0);
uint8_t DSP_MFN_Init(DSP_MFN_Obj *hFilter, uint32_t size);
void DSP_MFN_DeInit(DSP_MFN_Obj *hFilter, uint32_t size);
float DSP_MFN_Handle(DSP_MFN_Obj *hFilter, float newVal);

#endif