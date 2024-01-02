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
#ifndef CONSTANTS_H
#define	CONSTANTS_H

#include <xc.h> // include processor files - each processor file is guarded.  

// PWM and timer
#define FOSC 144000000
#define FCY FOSC/2
#define FPWM 10000
#define OCRS FCY/FPWM

// Control loop
#define MAX_TASKS 5
#define WAIT_FOR_START 0
#define MOVING 1
//#define MINTH 0.15
//#define MAXTH 0.30

// UART
#define UART_USE 1

// initial value 292
#define BUFFER_TX 305
#define BUFFER_RX 13
#define TRUE 1
#define FALSE 0


// ADC
#define VOLT_REF_IR 3.3
#define VOLT_REF_BATTERY 3.7
#define MAX_INT_ADC 1024


// timer
#define MAX_INT 65535
#define MAX_PERIOD 200
#define PRESCALE_SAT 256
#define PRESCALE_BIN_SAT 3
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
    

#endif	/* XC_HEADER_TEMPLATE_H */

