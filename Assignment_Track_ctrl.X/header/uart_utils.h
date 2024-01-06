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
#ifndef XC_UART_UTILS_H
#define	XC_UART_UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "timer_utils.h"
#define BAUD 38400

// function to perform input/output remapping and configuration based on the UART used 
void UART_config(int val){
    
    switch(val){
        case 1:
            RPOR0bits.RP64R = 0x01; // Map UART 1 TX to pin RD0 which is RP64
            RPINR18bits.U1RXR = 0x4B; // Map UART 1 RX to pin RD11 which is RPI75 (0x4B = 75)
            U1BRG = (long)FCY/(16L *(long)BAUD)-1; 
            U1MODEbits.UARTEN = 1; // enable UART
            U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
            U1STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
            U1STAbits.URXISEL = 0; // generate interrupt everytime a character is received
            //U1STAbits.UTXISEL0 = 0; // generate interrupt everytime a bit is empty in the transmit buffer
            IEC0bits.U1RXIE = 1; // enable uart 1 RX intterupt
            //IEC0bits.U1TXIE = 1; // enable uart 1 TX intterupt
            break;
        case 2:
            RPOR6bits.RP85R = 0x03; // Map UART 2 TX to pin RE5 which is RP85
            RPINR19bits.U2RXR = 0x56; // Map UART 2 RX to pin RE6 which is RPI86 (0x56 = 86)
            U2BRG = (long)FCY/(16L * (long)BAUD)-1; // = 11
            U2MODEbits.UARTEN = 1; // enable UART
            U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
            U2STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
            U2STAbits.URXISEL = 0; // generate interrupt everytime a character is received
            IEC1bits.U2RXIE = 1; // enable uart 2 RX intterupt
    }
}

#endif	/* XC_HEADER_TEMPLATE_H */

