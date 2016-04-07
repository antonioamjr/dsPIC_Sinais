#include <dsp.h>
#include "config.h"
#include "configADC.h"
#include <dsp_factors_32b.h>
//#include "configUART.h"

#define LED  	     _LATB15
#define LED_TRIS     _TRISB15
#define BUTTON       _RB14
#define BUTTON_TRIS  _TRISB14

// /////////////////////////////////////////////////////////////////////////
// Macros de registro para configuração do dispositivo
// /////////////////////////////////////////////////////////////////////////
_FOSCSEL(FNOSC_FRC & IESO_OFF);			// Internal FRC oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR64);
_FICD(JTAGEN_OFF);

// //////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variáveis globais
// /////////////////////////////////////////////////////////////////////////////////////////////////////////
_Bool         ready_flag = 0;
//float         modFFT[(NUMSAMP/2)+1];
//float         resultFrac[NUMSAMP/5];
//fractional    resultBuffer2[NUMSAMP];
fractional    resultBuffer[NUMSAMP];
fractcomplex  result[NUMSAMP] __attribute__ ((section ("ydata, ymemory"), aligned (NUMSAMP*2*2)));
fractcomplex  twidFactors[NUMSAMP/2] __attribute__ ((section (".xbss, bss, xmemory"), aligned (NUMSAMP*2)));
int	      peakFrequencyBin = 0;	// Declare post-FFT variables to compute the
unsigned long peakFrequency = 0;        // frequency of the largest spectral component

int main(void)
{
    // /////////////////////////////////////////////////////
    // FUNÇÔES DE INICIALIZAÇÃO
    // /////////////////////////////////////////////////////
    //initUART();
    initClock40MIPS();   
    initDma0();
    initADC1();

    // ///////////////////////////////////////////////////////
    // FUNÇÔES DE INICIALIZAÇÃO
    // ///////////////////////////////////////////////////////
    int i;
    LED_TRIS = 0;           //DIREÇÂO DO PINO DO LED
    LED = 0;                //VALOR INICIAL DO LED
    BUTTON_TRIS = 1;        //DIREÇÂO DO PINO DO PUSH_BUTTON

    // Inicialização dos Twiddle Factors usado no algoritmo DSP FFT
    TwidFactorInit (LOG2NUMSAMP, &twidFactors[0], 0);

    for(;;)
    {
        if(ready_flag)
        {
            //printADCSamples(resultBuffer,NUMSAMP);
            //dft(resultBuffer);

            // Sinal amostrado
            for(i=0;i<NUMSAMP;i++)
            {
                result[i].real = resultBuffer[i]>>1;
                result[i].imag = 0;
                //resultFrac[i] = resultFrac[i]/2;
            }

            // Executa a FFT com resultado em ordem de bits reverso
            FFTComplexIP(LOG2NUMSAMP,&result[0],&twidFactors[0],COEFFS_IN_DATA);

            //Resultado da FFT é colocado em ordem normal de bits
            BitReverseComplex (LOG2NUMSAMP, &result[0]);

            //Armazena a Magnitude complexa do resulta da FFT
            SquareMagnitudeCplx(NUMSAMP, &result[0], &resultBuffer[0]);        //TESTAR TESTAR TESTAR//

            //Pega o maior valor do vetor de resultados da FFT
            VectorMax(NUMSAMP/2, &resultBuffer[0], &peakFrequencyBin);

            //Frequência máxima calculada a partir do valor máximo percebido pela amostra do Sinal
            peakFrequency = peakFrequencyBin*((float)FREQADC/(float)NUMSAMP);
           
            
            ready_flag = 0;
        }

        if(BUTTON)
        {
            __delay32(FCY*0.001);
            if(BUTTON)
            {
                LED ^= 1;
                DmaBuffer = 0;
                AD1CON1bits.ADON = 1;
                __delay32(ADC_Dly_Time);
                AD1CON1bits.ASAM = 1;
                while(BUTTON){Nop();}
                
            }
        }
    }

}