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
#ifndef XC_SCHEDULER_UTILS_H
#define	XC_SCHEDULER_UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <math.h>
#include <stdio.h>
#include "uart_utils.h"
#include "ADC_utils.h"
#include "buffer_utils.h"
#include "PWM_utils.h"
#include "update_state_utils.h"
#include "constants.h"

int state = WAIT_FOR_START;
float dist;

typedef struct {
int n;
int N;
} heartbeat;

heartbeat schedInfo[MAX_TASKS];
char tmp_buffer[19];

void scheduler_init(){
    
    // Timer of 1ms, two tasks of 100ms (10 Hz) and one task of 1000ms (1Hz)
    buttonRE8_config(&state);
    
    int i;
    
    for(i=0; i <MAX_TASKS; i++){
        switch(i){
                case 0:
                    schedInfo[i].N = 100;
                    schedInfo[i].n = 0;
                    break;
                case 1:
                    schedInfo[i].N = 100;
                    schedInfo[i].n = 0;
                    break;
                case 2:
                    schedInfo[i].N = 1000;
                    schedInfo[i].n = 0;
                    break;
                case 3:
                    schedInfo[i].N = 100;
                    schedInfo[i].n = 0;
                    break;
                case 4:
                    schedInfo[i].N = 1;
                    schedInfo[i].n = 0;
                    break;
            }
    }
}

void task_motors(){
   
   int d1 = 1, d2 = 2, d3 = 3, d4 = 4; 
    
   //sprintf(tmp_buffer,"$MPWM,%d,%d,%d,%d*",d1,d2,d3,d4);
   //write_ring(tmp_buffer);
    
}

void task_IR(){
    
    int i = 0;
    float volt;
    
    if(AD1CON1bits.DONE){
        volt = ((float)ADC1BUF1/(float)(MAX_INT_ADC)) * VOLT_REF_IR;
        //volt = volt*100;
        dist = 2.34 - 4.74 * volt + 4.06 * powf(volt,2) - 1.60 * powf(volt,3) + 0.24 * powf(volt,4);
        
        //sprintf(tmp_buffer,"%.3f*",dist);
        //write_ring(tmp_buffer);
    }
}

void task_sendUART(){
    
    while(U1STAbits.UTXBF == 0)
        U1TXREG = read_ring();
    //LATAbits.LATA0 = !LATAbits.LATA0;
    
}

void task_battery(){
    
   float volt_battery;
   
   
   volt_battery = ((float)ADC1BUF0/(float)(MAX_INT_ADC)) * VOLT_REF_BATTERY;
        //U1TXREG = printf("battery: %f", volt*3);
        //U1TXREG = printf("distance: %f and volt: %f", dist,volt*3);
    
   //sprintf(tmp_buffer,"$MBATT,%.2f*",volt_read);
   //write_ring(tmp_buffer);

}

void task_control(){
    
    
    
    float surge_left, surge_right, yaw_rate;
    
    if(state == WAIT_FOR_START){
        shutdown_motors();
        //blink led A0 to do
    }
    
    else if(state == MOVING){
        
        LATAbits.LATA0 = 1;
        
        if(dist > MAXTH){
            surge_left = 1; 
            surge_right = 1;
            //forward
            forward(surge_left, surge_right); 
        }
            
            
        else if(dist <= MAXTH && dist >= MINTH){
            //proportional law combining forward and turning
            surge_left = 1; 
            surge_right = (dist - (float)MINTH) / (float)(MAXTH - MINTH);
            
            forward(surge_left, surge_right);
            
        }
            
            
        else if(dist < MINTH){
            // turning
            yaw_rate = 1;
            
            turn_right(yaw_rate);
        }
    }
    
    
}

void scheduler() {
    
    
    int i;
    
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if(schedInfo[i].n >= schedInfo[i].N){
            switch(i){
                case 0:
                    task_motors();
                    break;
                case 1:
                    task_IR();
                    break;
                case 2:
                    task_battery();
                    break;
                case 3:
                    task_sendUART();
                    break;
                case 4:
                    task_control();
                    break;
            }
            schedInfo[i].n = 0;
        }
    }
}

#endif	/* XC_HEADER_TEMPLATE_H */

