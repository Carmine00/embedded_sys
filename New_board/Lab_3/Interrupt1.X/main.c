/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023
 */


#include <xc.h>
#include "../../header/conf_bits.h"
#include "../../header/timer_utils.h"

void __attribute__ ((__interrupt__ , __auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0; // reset interrupt flag
    LATGbits.LATG9 = !LATGbits.LATG9; // toggle led

}

int main(void) {
    
    // all analog pins are set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    PLLFBD = 6; // M = 8
    CLKDIVbits.PLLPOST = 0x00; // N1 = 2
    CLKDIVbits.PLLPRE = 0x00; // N2 = 2
    RCONbits.SWDTEN = 0; // Disable Watch Dog Timer
    while (OSCCONbits.LOCK != 1) {};
    
    IEC0bits.T2IE = 1; // enable interrupt
    INTCON2bits.GIE = 1; 
    INTCON2bits.INT1EP = 1; 
    
    TRISAbits.TRISA0 = 0; // set A0 led as output pin
    LATAbits.LATA0 = 1;
    TRISGbits.TRISG9 = 0; // set G9 led as output pin
    LATGbits.LATG9 = 0;
    
    tmr_setup_period(TIMER2, 250);
            
    while(1){        
        
        // wait for the set period of time
        tmr_wait_ms(TIMER1, 2000);
        
        // toggle LED
        LATAbits.LATA0 = !LATAbits.LATA0;
    }
    
    
    return 0;
}
