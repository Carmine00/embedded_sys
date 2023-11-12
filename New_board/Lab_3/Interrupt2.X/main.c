/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023, 12.49
 */


#include <xc.h>
#include "../../header/conf_bits.h"
#include "../../header/timer_utils.h"

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT1Interrupt() {
    // INT1 is related to an external interrupt, in this case the click of button E8 (or E9)
    IEC1bits.INT1IE = 0; // disable interrupt button
    T2CONbits.TON = 1;
}

// interrupt for button bouncing
void __attribute__ ((__interrupt__ , __auto_psv__)) _T2CKInterrupt() {
    
    IFS0bits.T2IF = 0;
    
    // LATBbits.LATB1 = !LATBbits.LATB1; --> debug
        
    // check button status
    if(PORTEbits.RE8 != 0){
        // toggle led
        //LATAbits.LATA0 = 1; --> debug
        LATGbits.LATG9 = !LATGbits.LATG9;
    }
    T2CONbits.TON = 0; // disable timer
    TMR2 = 0; // reset counter for the timer because we don't know when this interrupt happens
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1; // able interrupt firing for button
}

int main(void) {
    
    // all analog pins are set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // remapping intterupt T1 to switch
    RPINR0bits.INT1R = 0x58; // 0x58 is 88 in hex (try 0x59 for 89 so RPI89 aka E9 switch)
    // INTCON2bits.GIE = 1; --> set gloabl interrupt enable ???
    // INTCON2bits.INT1EP = 1; --> interrupt on negative edge ???
    IEC1bits.INT1IE = 1; // enable external interrupt T1
    IEC0bits.T2IE = 1; // enable timer T2 interrupt
    /* in case of debug
    TRISAbits.TRISA0 = 0; // set A0 led as output pin
    LATAbits.LATA0 = 0;
    */
    TRISGbits.TRISG9 = 0; // set G9 led as output pin
    LATGbits.LATG9 = 0;
    TRISEbits.TRISE8 = 1; // set E8 switch (RPI88) as input pin
    // TRISEbits.TRISE9 = 1; // set E9 (RPI89) switch as input pin
    
    tmr_setup_period(TIMER1,500);
    tmr_setup_period(TIMER2,20);
                
    while(1){        
        
        // wait for the set period of time
        tmr_wait_period(TIMER1);
    }
    
    
    return 0;
}
