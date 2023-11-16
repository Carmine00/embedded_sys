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
#ifndef XC_INTERRUPT_HANDLE_H
#define	XC_INTERRUPT_HANDLE_H

#include <xc.h> // include processor files - each processor file is guarded.  

void __attribute__ ((__interrupt__ , __auto_psv__)) _U2RXInterrupt() {
    
    IFS1bits.U2RXIF = 0; // reset interrupt flag
    
    write_ring(read_UART());
    
    char_counter++;
    
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT0Interrupt(){
    
    
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 0;
    T3CONbits.TON = 1;
    state = STATE_S5-1; 
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT1Interrupt(){
    
    IFS1bits.INT1IF= 0;
    IEC1bits.INT1IE = 0;
    T3CONbits.TON = 1;
    state = STATE_S6-1;  
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _T3Interrupt(){
    
    IFS0bits.T3IF = 0;
    
    if(PORTEbits.RE8 && state == STATE_S5-1){
        state++;
        }
    
        else if(PORTDbits.RD0 && state == STATE_S6-1){
            state++;
        }
    
    TMR3 = 0;
    T3CONbits.TON = 0;
    
    IEC0bits.INT0IE = 1;
    IEC1bits.INT1IE = 1;
    
    IFS0bits.INT0IF = 0;
    IFS1bits.INT1IF = 0;
}

void interrupt_able(int val){
    
    /* enabble or disable the interrupts related to: 
     * INT0, INT1 and UART2
     */
    IEC0bits.INT0IE = val;
    IEC1bits.INT1IE = val;
    IEC1bits.U2RXIE = val;
}

#endif	/* XC_HEADER_TEMPLATE_H */

