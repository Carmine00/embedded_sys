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
#include <string.h>
#include "uart_utils.h"
#include "ADC_utils.h"
#include "buffer_utils.h"
#include "PWM_utils.h"
#include "update_state_utils.h"
#include "protocol_utils.h"
#include "constants.h"

int state = WAIT_FOR_START;
double dist, val_min, val_max;

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
                case 0:  // motors
                    schedInfo[i].N = 100; // 100 ms = 10 hz
                    schedInfo[i].n = 0;
                    break;
                case 1:  // ir sensor distance
                    schedInfo[i].N = 100;   // 100 ms = 10 hz
                    schedInfo[i].n = 0;
                    break;
                case 2:  // minth/maxth threshold update
                    schedInfo[i].N = 100;   // 100 ms = 10 hz
                    schedInfo[i].n = 0;
                    break;
                case 3: // battery
                    schedInfo[i].N = 1000; // 1000 ms = 1 hz
                    schedInfo[i].n = 0;
                    break;
                case 4: // control
                    schedInfo[i].N = 1;  // 1 ms = 1 khz
                    schedInfo[i].n = 0;
                    break;
            }
    }
}

void task_motors(){ 
    
   sprintf(tmp_buffer,"$MPWM,%d,%d,%d,%d*", OC1R, OC2R, OC3R, OC4R);
   write_ringTX(tmp_buffer);
   
   while(U1STAbits.UTXBF == 0) // meglio usare il TRMT e/o mettere un if invece di un while?
        U1TXREG = read_ringTX();
    
}

void task_IR(){
    
    double volt;
    
    if(AD1CON1bits.DONE){
        volt = ((double)ADC1BUF1/(double)(MAX_INT_ADC)) * VOLT_REF_IR;
        //volt = volt*100;
        dist = 2.34 - 4.74 * volt + 4.06 * powf(volt,2) - 1.60 * powf(volt,3) + 0.24 * powf(volt,4);
        
        sprintf(tmp_buffer,"%.3f*",dist);
        write_ringTX(tmp_buffer);
        
        while(U1STAbits.UTXBF == 0)
            U1TXREG = read_ringTX();
    }
}

void task_threshold(){
    
    char data_rcv;
    int ret, threshold_min, threshold_max; 
    parser_state pstate;
    pstate.state = STATE_DOLLAR;
    pstate.index_type = 0;
    pstate.index_payload = 0;
    
    
    while(data_RX.head != data_RX.tail){
            
            data_rcv = read_ringRX();
            ret = parse_byte(&pstate, data_rcv);
            if (ret == NEW_MESSAGE){
                if (strcmp(pstate.msg_type, MSG_INFO) == 0) {
                    sscanf(pstate.msg_payload,"%d,%d",&threshold_min,&threshold_max);
                    val_min = (double)threshold_min/100.0;
                    val_max = (double)threshold_max/100.0;
                    
                } else if (ret == ERR_MESSAGE)
                    write_ringTX(ack_err);
        }
    }
    
   
}

void task_battery(){
    
   double volt_battery;
   
   if(AD1CON1bits.DONE){
        volt_battery = ((double)ADC1BUF0/(double)(MAX_INT_ADC)) * VOLT_REF_BATTERY;
             //U1TXREG = printf("battery: %f", volt*3);
             //U1TXREG = printf("distance: %f and volt: %f", dist,volt*3);

        sprintf(tmp_buffer,"$MBATT,%.2f*",volt_battery * 3);
        write_ringTX(tmp_buffer);

        while(U1STAbits.UTXBF == 0)
            U1TXREG = read_ringTX();
    }
}

void task_control(){
    
    
    
    float surge_left, surge_right, yaw_rate;
    
    if(state == WAIT_FOR_START){
        shutdown_motors();
        //blink led A0 to do
    }
    
    else if(state == MOVING){
        
        LATAbits.LATA0 = 1;
        
        if(dist > val_max){
            surge_left = 1; 
            surge_right = 1;
            //forward
            forward(surge_left, surge_right); 
        }
            
            
        else if(dist <= val_max && dist >= val_min){
            //proportional law combining forward and turning
            surge_left = 1; 
            surge_right = (dist - (float)val_min) / (float)(val_max - val_min);
            
            forward(surge_left, surge_right);
            
        }
            
            
        else if(dist < val_min){
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
                    task_threshold();
                    break;
                case 3:
                    task_battery();
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

