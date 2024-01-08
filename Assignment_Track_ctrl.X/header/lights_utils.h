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
#ifndef LIGHTS_UTILS_H
#define	LIGHTS_UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "constants.h"

void lights_config(){
    // beam head light
    TRISAbits.TRISA7 = 0;
    LATAbits.LATA7 = 0;
    
    // low intensity lights
    TRISGbits.TRISG1 = 0;
    LATGbits.LATG1 = 0;
    
    // brakes
    TRISFbits.TRISF0 = 0;
    LATFbits.LATF0 = 0;
    
    // right side lights
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;
    
    // left side lights
    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8 = 0;
}

// function to set the state of rear and head lights
void head_rear_lights(const int state, const float surge, const float yaw){
    if (state == WAIT_FOR_START) {
        // beam head lights
        LATAbits.LATA7 = 0;
        // low intensity head lights
        LATGbits.LATG1 = 0;
        // brakes
        LATFbits.LATF0 = 0;
    }
    
    if (state == MOVING) {
        if (surge > 0.5) {
            // beam head lights
            LATAbits.LATA7 = 1;
            // low intensity head lights
            LATGbits.LATG1 = 0;
            // brakes
            LATFbits.LATF0 = 0;
        }
        
        if (surge < 0.5){
            // beam head lights
            LATAbits.LATA7 = 0;
            // low intensity head lights
            LATGbits.LATG1 = 1;
            // brakes
            LATFbits.LATF0 = 1;
        }
    }
}

//function to set the state of side lights
void side_lights(const int state, const float surge, const float yaw){
    if (state == WAIT_FOR_START) {
        // toggle left side light
        LATBbits.LATB8 = !PORTBbits.RB8;
        // toggle right side light as left side light to have the same level
        LATFbits.LATF1 = !PORTBbits.RB8;
    }
    
    if (state == MOVING) {
        if (yaw > 0.15) {
            // left side light
            LATBbits.LATB8 = 0;
            // right side light
            LATFbits.LATF1 = !LATFbits.LATF1;
        }
        
        if (yaw < 0.15) {
            // left side light
            LATBbits.LATB8 = 0;
            // right side light
            LATFbits.LATF1 = 0;
        }
    }
}
    
#endif	/* LIGHTS_UTILS_H */

