/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023
 */


#include <xc.h>
#include "../../header/conf_bits.h"
#include "../../header/timer_utils.h"

void __attribute__ ((__interrupt__ , __auto_psv__)) _T2CKInterrupt() {
    IFS0bits.T2IF = 0; // reset interrupt flag
    LATGbits.LATG9 = !LATGbits.LATG9; // toggle led

}

int main(void) {
    
    // all analog pins are set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    IEC0bits.T2IE = 1; // enable interrupt
    // INTCON2bits.GIE = 1; --> set gloabl interrupt enable ???
    // INTCON2bits.INT1EP = 1; --> interrupt on negative edge ???
    
    TRISAbits.TRISA0 = 0; // set A0 led as output pin
    LATAbits.LATA0 = 0;
    TRISGbits.TRISG9 = 0; // set G9 led as output pin
    LATGbits.LATG9 = 0;
    
    tmr_setup_period(TIMER2, 250);
            
    while(1){        
        
        // wait for the set period of time
        tmr_wait_ms(TIMER1, 500);
        
        // toggle LED
        LATAbits.LATA0 = !LATAbits.LATA0;
    }
    
    
    return 0;
}
