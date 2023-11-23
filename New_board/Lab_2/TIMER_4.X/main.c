/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 12 novembre 2023
 */


#include <xc.h>
#include "../../header/timer_utils.h"

int main(void) {
    
    // all analog pins are set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    TRISAbits.TRISA0 = 0; // set A0 (led) as output pin
    LATAbits.LATA0 = 0;
    
    TRISEbits.TRISE8 = 1; // set RE8 (button) as input pin, try RE9
    
    int button_old = PORTEbits.RE8, button_new, button_press = 0;
    
    while(1){
        button_new = PORTEbits.RE8;
        
        if (button_new < button_old) { // button pressed
            if(button_press < 3)
                button_press += 1;
            else
                button_press = 1;
            // start timer of 3s
            //tmr_setup_period(TIMER2, 3000);
        }
        /*if (button_new > button_old && IFS0bits.T2IF == 1){ // button released
            
            // check timer 3 second
            LATAbits.LATA0 = 0;
            button_press = 0;
            
        }*/
        button_old = button_new;
        
        if (button_press){
        for(int i = 0; i < 2*button_press -1; i++){
            // setup and start timer for pulses
            LATAbits.LATA0 = !LATAbits.LATA0;
            tmr_wait_ms(TIMER1, 100);
        }
        
        LATAbits.LATA0 = !LATAbits.LATA0;
        tmr_wait_ms(TIMER1, 1000 -(2*button_press-1)*100);
        }
    }
   
    return 0;
}
