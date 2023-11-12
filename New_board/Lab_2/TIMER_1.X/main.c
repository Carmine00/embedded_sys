/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023
 */

#include <xc.h>
#include "conf_bits.h"
#include "timer_utils.h"

int main(void) {
    
    // all pins set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // set led RA0 as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    tmr_setup_period(TIMER1, 1000);
        
    while(1){        
        
        // wait for the set period of time
        tmr_wait_period(TIMER1);
        
        // toggle LED
        LATAbits.LATA0 = !LATAbits.LATA0;
    }
    
    return 0;
}
