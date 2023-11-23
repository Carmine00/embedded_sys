/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 23 novembre 2023, 11.18
 */


#include <xc.h>
#include "../../header/uart_utils.h"
#define UART_USE 1


int main(void) {
    
    uart_config(UART_USE);
    
    AD1CON3bits.ADCS = 8; // 8*T_CY
    AD1CON1bits.ASAM = 0; // manual sampling
    AD1CON1bits.SSRC = 0; // manual end sampling, manual conversion
    AD1CON2bits.CHPS = 0; // use CH0
    AD1CHS0bits.CH0SA = 0;
    //AD1CH123Sbits.CH123SA = 0;
    ANSELA = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    ANSELBbits.ANSB5 = 1; // see schematic, output V0
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 1; // enable EN pin slider
    AD1CON1bits.ADON = 1;
    
    ADCBUFF
    
    
    while(1){}
    
    return 0;
}
