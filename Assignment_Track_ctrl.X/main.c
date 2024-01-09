 /*
 * File:   main.c
 * Authors: Luca Petruzzello (s5673449), Carmine Miceli (s5626492), Davide Cattin (s5544178)
 *
 * Created on 10 December 2023
 */


#include <xc.h>
#include "header/timer_utils.h"
#include "header/uart_utils.h"
#include "header/ADC_utils.h"
#include "header/buffer_utils.h"
#include "header/scheduler_utils.h"
#include "header/PWM_utils.h"
#include "header/update_state_utils.h"
#include "header/lights_utils.h"
#include "header/protocol_utils.h"
#include "header/constants.h"

int main(void) {
    
    // all pins set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // set led RA0 as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    UART_config(UART_USE);
    ADC_config();
    PWM_config();
    lights_config();
    protocol_init();
    scheduler_init();
    
    while(1){    
        scheduler();
        tmr_wait_ms(TIMER1, 1);
    }
    
    return 0;
}
