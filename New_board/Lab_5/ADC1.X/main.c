/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 23 novembre 2023, 11.18
 */

#include <stdio.h>
#include <math.h>
#include <xc.h>
#include "../../header/uart_utils.h"
#include "../../header/timer_utils.h"
#define UART_USE 1
#define VOLT_REF 3.3
#define MAX_INT_ADC 1024
int main(void) {
    
    float dist, volt;
    UART_config(UART_USE);
    
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    
    TRISBbits.TRISB14 = 1;
    ANSELBbits.ANSB14 = 1; // see schematic, output V0 (analog channel)
    
    AD1CON3bits.ADCS = 14; // 14*T_CY
    AD1CON1bits.ASAM = 0; // manual sampling
    //AD1CON1bits.SAMP = 1;
    AD1CON1bits.SSRC = 7; // automatic conversion
    AD1CON3bits.SAMC = 16; // sampling lasts 16 Tad
    AD1CON2bits.CHPS = 0; // use CH0
    AD1CHS0bits.CH0SA = 14; // remapping  
    //AD1CHS123bits.CH123SA = 1;
    
    
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 1; // enable EN pin 
    AD1CON1bits.ADON = 1;
    
    //U1TXREG = 'c';

    
    while(1){
        //U1TXREG = 'd';
        AD1CON1bits.SAMP = 1; // manual sampling
        while (!AD1CON1bits.DONE);
        //U1TXREG = 'e';
        
        volt = ((float)ADC1BUF0/(float)(MAX_INT_ADC)) * VOLT_REF;
        //volt = volt*100;
        dist = 2.34 - 4.74 * volt + 4.06 * powf(volt,2) - 1.60 * powf(volt,3) + 0.24 * powf(volt,4);
        
        U1TXREG = printf("%f", dist) ;

        tmr_wait_ms(TIMER1, 1000);
    }
    
    return 0;
}
