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
#ifndef XC_BUFFER_UTILS_H
#define	XC_BUFFER_UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "constants.h"

typedef struct{
    char buf[BUFFER_RX];
    int head;
    int tail;
} RingBufferRX;

// dedicated buffer for receveing
RingBufferRX data_RX;

void init_ring_buffer(){
    
    data_RX.head = 0;
    data_RX.tail = 0;
}

void write_ringRX(char data){
    
    // insert one character into ring buffer 
    
    data_RX.buf[data_RX.head] = data;
    data_RX.head = (data_RX.head + 1) % BUFFER_RX;
    
}

char read_ringRX(){
    
    // extract one character from ring buffer 
    
    char data;
    
    data = data_RX.buf[data_RX.tail];
    data_RX.tail = (data_RX.tail + 1) % BUFFER_RX;
    
    return data;
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _U1RXInterrupt() {
    IFS0bits.U1RXIF = 0; // reset interrupt flag
    
    write_ringRX((char)U1RXREG);  // register received character into ring RX buffer
}

#endif	/* XC_HEADER_TEMPLATE_H */

