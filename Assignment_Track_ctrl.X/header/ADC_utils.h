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
#ifndef XC_ADC_UTILS_H
#define	XC_ADC_UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "constants.h"

void ADC_config(){
    
    TRISBbits.TRISB15 = 1;
    ANSELBbits.ANSB15 = 1;  // ir sensor Vout (analog channel) for mikrobus socket one
    
    TRISAbits.TRISA3 = 0;
    LATAbits.LATA3 = 1;   // enable EN pin mikrobus socket one
    
    TRISBbits.TRISB11 = 1;
    ANSELBbits.ANSB11 = 1; // battery Vout / 3 (analog channel)
    
    AD1CON3bits.ADCS = 14; // 14*T_CY
    AD1CON1bits.ASAM = 1; // automatic sampling
    AD1CON2bits.CSCNA = 1; // scan mode enabled
    AD1CSSLbits.CSS11 = 1;   // scan for AN11 battery
    AD1CSSLbits.CSS15 = 1;   // scan for AN15 ir sensor
    
    AD1CON1bits.SSRC = 7; // automatic conversion
    AD1CON3bits.SAMC = 16; // sampling lasts 16 Tad
    AD1CON2bits.CHPS = 0; // use CH0 2-channels sequential sampling mode
    AD1CON2bits.SMPI = 1; // N-1 channels
    AD1CON1bits.SIMSAM = 0; // sequential sampling
    
    AD1CON1bits.ADON = 1;

}

#endif	/* XC_HEADER_TEMPLATE_H */

