#include "dsp_filters.h"
#include <math.h>

// Функция Инициализации фильтра первого порядка 
void DSP_LPF1_Init(DSP_LPF1_Obj *hFilter, float kx0, float kx1, float ky1)
{
    hFilter->KX0 = kx0;
    hFilter->KX1 = kx1;
    hFilter->KY1 = ky1;
	hFilter->X1 = 0;
	hFilter->Y1 = 0;
}

// Фунция обработки фильтра первого порядка
float DSP_LPF1_Handle(DSP_LPF1_Obj *hFilter, float X0)
{
    float Y0 = 0.0f;
    Y0 = hFilter->KX0 * X0 + hFilter->KX1 * hFilter->X1 + hFilter->KY1 * hFilter->Y1;
    hFilter->Y1 = Y0;
    hFilter->X1 = X0;
    return Y0;
}

// Функция инициализации медианного фильтра для N значений
void DSP_MFN_Init(DSP_MFN_Obj *hFilter, float *buf, uint32_t size)
{
    hFilter->Size = size;
    hFilter->Buf = buf;
}

// Функция деинициализации медианного фильтра для N значений
void DSP_MFN_DeInit(DSP_MFN_Obj *hFilter)
{
    memset(hFilter, 0, sizeof(DSP_MFN_Obj));
}

// Функция обработки медианного фильтра для N значений
float DSP_MFN_Handle(DSP_MFN_Obj *hFilter, float newVal)
{
	float *buffer = hFilter->Buf;
	buffer[hFilter->Cnt] = newVal;
	if ((hFilter->Cnt < hFilter->Size - 1) && (buffer[hFilter->Cnt] > buffer[hFilter->Cnt + 1])) 
	{
		for (int i = hFilter->Cnt; i < hFilter->Size - 1; i++) 
		{
			if (buffer[i] > buffer[i + 1]) 
			{
				float buff = buffer[i];
				buffer[i] = buffer[i + 1];
				buffer[i + 1] = buff;
			}
		}
	}
	else 
	{
		if ((hFilter->Cnt > 0) && (buffer[hFilter->Cnt - 1] > buffer[hFilter->Cnt])) 
		{
			for (int i = hFilter->Cnt; i > 0; i--) 
			{
				if (buffer[i] < buffer[i - 1])
				{
					float buff = buffer[i];
					buffer[i] = buffer[i - 1];
					buffer[i - 1] = buff;
				}
			}
		}
	}
	hFilter->Cnt = (hFilter->Cnt + 1) % hFilter->Size;

	return buffer[(int)hFilter->Size / 2];
}

// Функция обработки медианного фильтра для 3 значений
float DSP_MF3_Handle(DSP_MF3_Obj *hFilter, float newVal)
{
	hFilter->Buf[hFilter->Cnt] = newVal;
	hFilter->Cnt = (hFilter->Cnt + 1) % 3;

	float a = hFilter->Buf[0];
	float b = hFilter->Buf[1];
	float c = hFilter->Buf[2];

	float middle;

	if ((a <= b) && (a <= c))
	{
		middle = (b <= c) ? b : c;
	}
	else
	{
		if ((b <= a) && (b <= c))
		{
			middle = (a <= c) ? a : c;
		}
		else
		{
			middle = (a <= b) ? a : b;
		}
	}
	return middle;
}



float DSP_Kalman_Init(DSP_Kalman_Obj *hFilter, float err_measure, float q)
{
	hFilter->Err_Measure = err_measure;
	hFilter->Q = q;
	hFilter->Err_Estimate = hFilter->Err_Measure;
	hFilter->Last_Estimate = 0.0f;
}


float DSP_Kalman_Handle(DSP_Kalman_Obj *hFilter, float newVal)
{
	float kalman_gain, current_estimate;

	kalman_gain = (float) hFilter->Err_Estimate / (hFilter->Err_Estimate  + hFilter->Err_Measure);
	current_estimate = hFilter->Last_Estimate + (float) kalman_gain * (newVal - hFilter->Last_Estimate);
	hFilter->Err_Estimate =  (1.0 - kalman_gain) * hFilter->Err_Estimate + fabs(hFilter->Last_Estimate - current_estimate) * hFilter->Q;
	hFilter->Last_Estimate = current_estimate;
	return current_estimate;
}
