/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef XC_UPDATE_STATE_UTILS_H
#define	XC_UPDATE_STATE_UTILS_H

#include <xc.h>
#include "timer_utils.h"

int *state_internal;

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT1Interrupt() {
    // INT1 is related to an external interrupt, in this case the click of button E8 (or E9)
    IEC1bits.INT1IE = 0; // disable interrupt button
    T3CONbits.TON = 1;
}

// interrupt for button bouncing
void __attribute__ ((__interrupt__ , __auto_psv__)) _T3Interrupt() {
    
    IFS0bits.T3IF = 0;
    
    if(PORTEbits.RE8 == 0){    // check button status
        if (*state_internal == MOVING)
            *state_internal = WAIT_FOR_START;
         else if (*state_internal == WAIT_FOR_START)
            *state_internal = MOVING;
    }
    
    //TRISBbits.TRISB1 = 0;
    //LATBbits.LATB1 = 1; //--> debug
        
    //LATAbits.LATA0 = 1;
    
    T3CONbits.TON = 0; // disable timer
    TMR3 = 0; // reset counter for the timer because we don't know when this interrupt happens
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1; // able interrupt firing for button
}


void buttonRE8_config(int *state){
    
    state_internal = state;
    
    RPINR0bits.INT1R = 0x58; // 0x58 is 88 in hex aka E8 switch
    INTCON2bits.GIE = 1; // set gloabl interrupt enable 
    INTCON2bits.INT1EP = 1; // interrupt on negative edge 
    IEC1bits.INT1IE = 1; // enable external interrupt T1
    
    tmr_setup_period(TIMER3,20);
    
}



#endif	/* XC_HEADER_TEMPLATE_H */

