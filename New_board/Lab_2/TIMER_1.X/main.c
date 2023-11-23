/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023
 */

#include <xc.h>
#include "../../header/timer_utils.h"

int main(void) {
    
    // all pins set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    
    // set led RA0 as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    int i = 0;
    tmr_setup_period(TIMER1, 200);
 
        
    while(1){        
        
        // wait for the set period of time
        tmr_wait_period(TIMER1);
        i = i +1;
        
        // toggle LED        
        if(i == 10){ // 2000 ms
            LATAbits.LATA0 = !LATAbits.LATA0;
            i = 0;
        }          
    }
    
   
    
    return 0;
}
