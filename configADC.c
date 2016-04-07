#include "configADC.h"

int BufferA[NUMSAMP/2] __attribute__((space(dma)));
int BufferB[NUMSAMP/2] __attribute__((space(dma)));

void initADC1(void)
{

    AD1CON1bits.FORM = 3;//3;		// Data Output Format: "Signed" Fraction (Q15 format)
    AD1CON1bits.SSRC = 7;//0;		// Internal counter ends sampling and starts conversion (auto-convert)
    AD1CON1bits.ASAM = 0;		// Sampling begins when SAMP bit is set (for now)
    AD1CON1bits.AD12B = 0;		// 10-bit ADC operation
    AD1CON1bits.ADDMABM = 1;            // DMA buffers are built in conversion order mode

    AD1CON2bits.BUFM = 0;
    AD1CON2bits.CSCNA = 1;

    AD1CON2bits.SMPI  = 0;		// SMPI must be 0
    AD1CON2bits.CHPS  = 0;//1;		// Converts CH0   NOT/CH1

    AD1CON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock
    AD1CON3bits.SAMC = 1;		// Auto Sample Time = 2*Tad
    AD1CON3bits.ADCS = 2;		// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*3 = 75ns (13.3Mhz)
							       // ADC Conversion Time for 10-bit Tc=12*Tad =  900ns (1.1MHz)

    //**  AD1CHS0/AD1CHS123: A/D Input Select Register  **//
    AD1CHS0bits.CH0SA = 0;		// MUXA +ve input selection (AN0) for CH0
    AD1CHS0bits.CH0NA = 0;		// MUXA -ve input selection (Vref-) for CH0

   // AD1CHS123bits.CH123SA = 0;	// MUXA +ve input selection (AN0) for CH1
   // AD1CHS123bits.CH123NA = 0;	// MUXA -ve input selection (Vref-) for CH1

    AD1CSSLbits.CSS0 = 1;

    //**  AD1PCFGH/AD1PCFGL: Port Configuration Register  **//
    //AD2PCFGL=0xFFFF;	// Set as digital
    AD1PCFGL=0xFFFF;	// Set as digital
    //AD1PCFGH=0xFFFF;	// Set as digital
    AD1PCFGLbits.PCFG0 = 0;		// AN0 as Analog Input


    IFS0bits.AD1IF = 0;			// Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 0;			// Do Not Enable A/D interrupt

    //AD1CON1bits.ADON = 1;		// Turn on the A/D converter

    __delay32(ADC_Dly_Time);			// Delay for 20uS to allow ADC to settle (25nS * 0x320 = 20uS)

    //AD1CON1bits.ASAM = 1;
}

void initDma0(void)
{
    _TRISB6 = 0;
    _LATB6 = 0;

    DMA0CONbits.AMODE = 0;			// Configure DMA for Register indirect with post increment
    DMA0CONbits.MODE  = 2;			// Configure DMA for Continuous Ping-Pong mode

    DMA0PAD=(volatile unsigned int)&ADC1BUF0;
    DMA0CNT=((NUMSAMP/2)-1);

    DMA0REQ=13;

    DMA0STA = __builtin_dmaoffset(BufferA);
    DMA0STB = __builtin_dmaoffset(BufferB);

    IFS0bits.DMA0IF = 0;			//Clear the DMA interrupt flag bit
    IEC0bits.DMA0IE = 1;			//Set the DMA interrupt enable bit

    DMA0CONbits.CHEN=1;
}

unsigned int DmaBuffer = 0;

void __attribute__((interrupt,no_auto_psv)) _DMA0Interrupt(void)
{
      //AD1CON1bits.ADON = 0;
//    AD1CON1bits.ASAM = 0; 

    if(DmaBuffer == 0)
    {
        DmaBuffer = 1;
//        AD1CON1bits.ADON = 1;
//        __delay32(ADC_Dly_Time);
//        AD1CON1bits.ASAM = 1;
    }
    else if(DmaBuffer == 1){
        AD1CON1bits.ADON = 0;
        AD1CON1bits.ASAM = 0;
        //DMA0CONbits.CHEN=1;
        ProcessADCSamples(BufferA,BufferB);
        DmaBuffer = 2;
    }

    //DmaBuffer ^= 1;

    //__builtin_btg((unsigned int *)&LATB, 6); // Toggle RB6

    IFS0bits.DMA0IF = 0;

}

void ProcessADCSamples(int* AdcBuffer,int* AdcBuffer2)
{
    /* Do something with ADC Samples */
    register unsigned int i;
    extern _Bool ready_flag;
    extern int resultBuffer[NUMSAMP];
    //extern float resultFrac[NUMSAMP];

    for(i = 0; i < NUMSAMP/2; i++){
        resultBuffer[i] = *AdcBuffer++;
        //resultFrac[i] = Q15toFloat(resultBuffer[i]);
    }
    for(i=NUMSAMP/2;i<NUMSAMP;i++)
    {
        resultBuffer[i] = *AdcBuffer2++;
        //resultFrac[i] = Q15toFloat(resultBuffer[i]);
    }
    ready_flag = 1;

}

