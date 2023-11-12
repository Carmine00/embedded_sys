/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023, 11.49
 */


#include <xc.h>
#include "../../header/conf_bits.h"
#include "../../header/timer_utils.h"

int main(void) {
    
   // all analog pins are set to digital
   ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
   // set led as output
   TRISAbits.TRISA0 = 0;
   LATAbits.LATA0 = 0;
    
   while(1){
         
        // toggle LED --> first turn on
        LATAbits.LATA0 = !LATAbits.LATA0;
        
        // set timer and wait for period
        tmr_wait_ms(TIMER1, 1000);

        // toggle LED --> turn off
        LATAbits.LATA0 = !LATAbits.LATA0;
        
        // set timer and wait for period
        tmr_wait_ms(TIMER1, 5000);

        // toggle LED
        LATAbits.LATA0 = !LATAbits.LATA0;
        
        // set timer and wait for period
        tmr_wait_ms(TIMER1, 500);
    }
    return 0;
}
