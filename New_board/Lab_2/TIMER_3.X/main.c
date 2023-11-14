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
    
    // all analog pins are set to digital
   ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
   // set led as output
   TRISAbits.TRISA0 = 0;
   LATAbits.LATA0 = 0;
   
   tmr_setup_period(TIMER1, 1000);
   int delay = 250;
    
    while(1){
        tmr_wait_ms(TIMER2, delay);
        
        LATAbits.LATA0 = !LATAbits.LATA0;
        
        tmr_wait_period(TIMER1);
    }
   
    return 0;
}
