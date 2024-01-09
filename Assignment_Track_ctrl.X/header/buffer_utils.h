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
    char buf[BUFFER_TX];
    int head;
    int tail;
} RingBufferTX;

typedef struct{
    char buf[BUFFER_RX];
    int head;
    int tail;
} RingBufferRX;

// dedicated buffer for transmission
RingBufferTX data_TX;
// dedicated buffer for receveing
RingBufferRX data_RX;

void init_ring_buffer(){
    
    data_TX.head = 0;
    data_TX.tail = 0;
    
    data_RX.head = 0;
    data_RX.tail = 0;
}

void write_ringTX(char *sensor_data){
    
   
    
    int i=0;
    
    while(sensor_data[i]!='\0'){
        data_TX.buf[data_TX.head] = sensor_data[i];
        data_TX.head = (data_TX.head + 1) % BUFFER_TX;
        i++;
    }

    
}

char read_ringTX(){
    
    
    char data;
    
    data = data_TX.buf[data_TX.tail];
    data_TX.tail = (data_TX.tail + 1) % BUFFER_TX;
    
    
    return data;
}

void write_ringRX(char data){
    
    
    data_RX.buf[data_RX.head] = data;
    data_RX.head = (data_RX.head + 1) % BUFFER_RX;
    
}

char read_ringRX(){
    
    
    char data;
    
    data = data_RX.buf[data_RX.tail];
    data_RX.tail = (data_RX.tail + 1) % BUFFER_RX;
    
    return data;
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _U1RXInterrupt() {
    IFS0bits.U1RXIF = 0; // reset interrupt flag
    
    write_ringRX((char)U1RXREG); 
}

#endif	/* XC_HEADER_TEMPLATE_H */

