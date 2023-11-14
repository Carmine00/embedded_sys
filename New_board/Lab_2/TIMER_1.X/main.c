/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023
 */

#include <xc.h>
#include "../../header/conf_bits.h"
#include "../../header/timer_utils.h"

int main(void) {
    
    // all pins set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    PLLFBD = 6; // M = 8
    CLKDIVbits.PLLPOST = 0x00; // N1 = 2
    CLKDIVbits.PLLPRE = 0x00; // N2 = 2
    RCONbits.SWDTEN = 0; // Disable Watch Dog Timer
    while (OSCCONbits.LOCK != 1) {};    
    
    // set led RA0 as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    tmr_setup_period(TIMER1, 2000);
 
        
    while(1){        
        
        // wait for the set period of time
        tmr_wait_period(TIMER1);
        
        // toggle LED
        LATAbits.LATA0 = !LATAbits.LATA0;
    }
    
   
    
    return 0;
}
