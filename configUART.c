#include "configUART.h"

/*void initUART(void){

    PPSUnLock;
    PPSOutput(OUT_FN_PPS_U1TX, OUT_PIN_PPS_RP9);
    iPPSInput(IN_FN_PPS_U1RX, IN_PIN_PPS_RP8);
    PPSLock;

    //__delay32(FCY*0.5);

    CloseUART1();

    U1MODEbits.UARTEN = 0;
    U1MODEbits.STSEL = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.BRGH = 0;

    U1BRG = UBRG;

    //U1STAbits.UTXISEL0 = 0;
    //U1STAbits.UTXISEL1 = 0;
    //IEC0bits.U1TXIE = 1;

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;


    __delay32(FCY);

}*/


// Print ADC samples by UART1
/*void printADCSamples(register int * AdcBuffer, int size){

    register unsigned int i;
    char convert[20];
    extern _Bool ready_flag;

    for(i = 0; i < size; i++){
        putsUART1((unsigned int *) "\r Sample ");
        putsUART1((unsigned int *) itoa(convert,i + 1,10));
        putsUART1((unsigned int *) ": ");
        while(BusyUART1()){Nop();}

        putsUART1((unsigned int *) itoa(convert,*AdcBuffer,10));
        putsUART1((unsigned int *) "\n");
        while(BusyUART1()){Nop();}
        AdcBuffer++;
    }

    putsUART1((unsigned int *) "\n");
    ready_flag = 0;
}*/

// Print FFT's complex numbers
/*void printComplex(register float * AdcBuffer, int size)
{
    register unsigned int i;
    char convert[20];
    extern _Bool ready_flag;

    for(i = 0; i < size; i++){

        putsUART1((unsigned int *) "\r Sample ");
        putsUART1((unsigned int *) itoa(convert,i + 1,10));
        putsUART1((unsigned int *) ": ");
        while(BusyUART1()){Nop();}

        //putsUART1((unsigned int *) ltoa(convert,*AdcBuffer,10));
        sprintf(convert,"%.5f",*AdcBuffer);
        putsUART1((unsigned int *) convert);
        putsUART1((unsigned int *) "\n");
        while(BusyUART1()){Nop();}
        AdcBuffer++;
    }

    putsUART1((unsigned int *) "\n");
    ready_flag = 0;
}*/