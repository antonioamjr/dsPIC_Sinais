#include "config.h"
#include "configADC.h"

// FRC with PLL: 40 MIPS
// FRC with PLL: (((7,37/(PRE+2))*(FBD+2))/(2*(POST+1))) MIPS
void initClock40MIPS(void)
{
    PLLFBD = 63;                // M = PLLFBD + 2
    CLKDIVbits.PLLPOST = 0;      // N2 = 2*(PLLPOST+1)
    CLKDIVbits.PLLPRE = 1;        // N1 = PLLPRE + 2

    __builtin_write_OSCCONH(0x01);		// Initiate Clock Switch 												// FRC with PLL (NOSC=0b001)
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while(OSCCONbits.COSC != 0b001){Nop();}	// Wait for Clock switch to occur
    while(OSCCONbits.LOCK != 1){Nop();}
}

/*void dft(register int * AdcBuffer)
{  
    float                 real[(NUMSAMP/2)+1];
    float                 imag[(NUMSAMP/2)+1];
    //float                 peakFreq[2] = {0,0};
    extern float          modFFT[(NUMSAMP/2)+1];
    //extern unsigned long  peakFrequency;
    register unsigned int k,i;
    register const float  pi2 = 2*(float)PI;
    //register const float  fator = ((float)FREQADC/(float)(NUMSAMP));

    for(k=0;k<(NUMSAMP/2)+1;k++)
    {
        real[k] = 0;
        imag[k] = 0;
    }

    for(k=0;k<(NUMSAMP/2)+1;k++)
    {
        real[k] = 0;
        imag[k] = 0;
        for(i=0;i<NUMSAMP;i++)
        {
            real[k] = real[k] + AdcBuffer[i] * cosf(pi2*k*i/NUMSAMP);
            imag[k] = imag[k] - AdcBuffer[i] * sinf(pi2*k*i/NUMSAMP);
        }

        modFFT[k] = sqrt(pow(real[k],2) + pow(imag[k],2));
        //if(peakFreq[0] < modFFT[k] && k != 0){peakFreq[0] = modFFT[k]; peakFreq[1] = k;}
    }
    //peakFrequency = peakFreq[1] * fator;
}*/