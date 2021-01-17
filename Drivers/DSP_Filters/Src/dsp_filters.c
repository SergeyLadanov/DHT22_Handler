#include "dsp_filters.h"

// ������� ������������� ������� ������� ������� 
void DSP_LPF1_Init(DSP_LPF1_Obj *hFilter, float kx0, float kx1, float ky1)
{
    hFilter->KX0 = kx0;
    hFilter->KX1 = kx1;
    hFilter->KY1 = ky1;
	hFilter->X1 = 0;
	hFilter->Y1 = 0;
}

// ������ ��������� ������� ������� �������
float DSP_LPF1_Handle(DSP_LPF1_Obj *hFilter, float X0)
{
    float Y0 = 0.0f;
    Y0 = hFilter->KX0 * X0 + hFilter->KX1 * hFilter->X1 + hFilter->KY1 * hFilter->Y1;
    hFilter->Y1 = Y0;
    hFilter->X1 = X0;
    return Y0;
}

// ������� ������������� ���������� ������� ��� N ��������
uint8_t DSP_MFN_Init(DSP_MFN_Obj *hFilter, uint32_t size)
{
    hFilter->Size = size;
    hFilter->Buf = (float *) malloc(sizeof(float) * hFilter->Size);

    if (hFilter->Buf == NULL)
    {
        printf("Allocation error for DSP_MFN!\r\n");
        return 1;
    }

    return 0;
}

// ������� ��������������� ���������� ������� ��� N ��������
void DSP_MFN_DeInit(DSP_MFN_Obj *hFilter, uint32_t size)
{
    hFilter->Size = 0;
    free(hFilter->Buf);
}

// ������� ��������� ���������� ������� ��� N ��������
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