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
    char buf[BUFFER];
    int head;
    int tail;
} RingBuffer;

RingBuffer data_r;

void init_ring_buffer(){
    
    data_r.head = 0;
    data_r.tail = 0;
}

void write_ring(char *data_tx){
    
    // atomic access to buffer
    //IEC0bits.U1RXIE = 0;
    int i=0;
    
    while(data_tx[i]!='\0'){
        data_r.buf[data_r.head] = data_tx[i];
        data_r.head = (data_r.head + 1) % BUFFER;
        i++;
    }
    
    //IEC0bits.U1RXIE = 1;
    
}

char read_ring(){
    
    // atomic access to buffer
    //IEC0bits.U1RXIE = 0;
    
    char data;
    
    data = data_r.buf[data_r.tail];
    data_r.tail = (data_r.tail + 1) % BUFFER;
    
    //IEC0bits.U1RXIE = 1;
    
    return data;
}

#endif	/* XC_HEADER_TEMPLATE_H */

