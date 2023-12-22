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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_TIMER_UTILS_H
#define	XC_TIMER_UTILS_H

#include "constants.h"

void configure_bit(int timer, int ms, int prescale, int prescale_binary){
    
    switch(timer){
        case TIMER1:
            // reset timer
            TMR1 = 0;
            
            // set period register
            PR1 = (uint16_t)((float)ms / 1000.0 * FCY / (float)prescale);
            
            // configure timer
            T1CONbits.TON = 0;  // disable timer
            T1CONbits.TCKPS = prescale_binary; // set prescale
            T1CONbits.TCS = 0;  // external clock
            T1CONbits.TGATE = 0;
            
            // reset interrupt timer 1
            IFS0bits.T1IF = 0;
            //IEC0bits.T1IE = 1; // enable interrupt
            
            T1CONbits.TON = 1;
            break;
        
        case TIMER2:
            // reset timer
            TMR2 = 0;
            
            // set period register
            PR2 = (uint16_t)((float)ms / 1000.0 * FCY / (float)prescale);
            
            // configure timer
            T2CONbits.TON = 0;  // disable timer
            T2CONbits.TCKPS = prescale_binary; // set prescale
            T2CONbits.TCS = 0;  // external clock
            T2CONbits.TGATE = 0;
            T2CONbits.T32 = 0;
            
            // reset interrupt timer 2
            IFS0bits.T2IF = 0;
            IEC0bits.T2IE = 1; // enable interrupt
            
            
            T2CONbits.TON = 1;
            break;
        
        case TIMER3:
            // reset timer
            TMR3 = 0;
            
            // set period register
            PR3 = (uint16_t)((float)ms / 1000.0 * FCY / (float)prescale);
            
            // configure timer
            T3CONbits.TON = 0;  // disable timer
            T3CONbits.TCKPS = prescale_binary; // set prescale
            T3CONbits.TCS = 0;  // external clock
            T3CONbits.TGATE = 0;
            
            // reset interrupt timer 3
            IFS0bits.T3IF = 0;
            IEC0bits.T3IE = 1; // enable interrupt
            
            T3CONbits.TON = 1;
            break;
    }
}

float tmr_setup_period(int timer, int ms) {
    
    // set prescale
    float prescale_f;
    // maxinum loop cycle in 200ms
    float num_cycle = 0.0;
    int prescale, prescale_binary;
    
    if(ms <= MAX_PERIOD){
        prescale_f = (float)ms / 1000.0 * FCY / (float)MAX_INT;
    if (prescale_f <= 1){
        prescale = 1;
        prescale_binary = 0;
    }
    else if (prescale_f <= 8){
        prescale = 8;
        prescale_binary = 1;
    }
    else if (prescale_f <= 64){
        prescale = 64;
        prescale_binary = 2;
    }
    else if (prescale_f <= 256){
        prescale = 256;
        prescale_binary = 3;
    }
    configure_bit(timer, ms, prescale, prescale_binary);  
   } else{
        prescale = PRESCALE_SAT;
        prescale_binary = PRESCALE_BIN_SAT;
        num_cycle = (float)ms / MAX_PERIOD;
        configure_bit(timer, MAX_PERIOD, prescale, prescale_binary);
    }
    
    return num_cycle;
}

void tmr_wait_period(int timer){
    switch(timer){
        case TIMER1:
            while (IFS0bits.T1IF == 0){}
            IFS0bits.T1IF = 0;
            break;
            
        case TIMER2:
            while (IFS0bits.T2IF == 0) {}
            IFS0bits.T2IF = 0;
            break;
        
        case TIMER3:
            while (IFS0bits.T3IF == 0) {}
            IFS0bits.T3IF = 0;
            break;
    }
}

void tmr_wait_ms(int timer, int ms) {
    
    float flag, diff;
    int i = 0;
    flag = tmr_setup_period(timer,ms);
    
    if (flag == 0.0)
        tmr_wait_period(timer);
    else{
        diff = flag - (int)flag;
        while(i != (int)flag){
            tmr_wait_period(timer);
            i = i + 1;
        }
        if(diff != 0.0)
            tmr_wait_ms(timer, (int)(ms*diff));
    }
}


#endif	/* XC_HEADER_TEMPLATE_H */

