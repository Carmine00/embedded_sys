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
#include "lights_utils.h"
#include "constants.h"
#include "protocol_utils.h"

int state = WAIT_FOR_START;
float dist = 0, val_min = MINTH, val_max = MAXTH;
float surge = 0;
float yaw = 0;

typedef struct {
int n;
int N;
} heartbeat;

heartbeat schedInfo[MAX_TASKS];
char tmp_buffer[25];

void scheduler_init(){
    
    // Timer of 1ms, two tasks of 100ms (10 Hz) and one task of 1000ms (1Hz)
    buttonRE8_config(&state);
    
    int i;
    
    for(i=0; i <MAX_TASKS; i++){
        switch(i){
            case 0: // duty cycle
                schedInfo[i].N = 100;
                schedInfo[i].n = 0;
                break;
                
            case 1: // IR sensor
                schedInfo[i].N = 100;
                schedInfo[i].n = 0;
                break;
                
            case 2: // battery
                schedInfo[i].N = 1000;
                schedInfo[i].n = 0;
                break;
            
            case 3:  // minth/maxth threshold update
                schedInfo[i].N = 100;   // 100 ms = 10 hz
                schedInfo[i].n = 0;
                break;
                
            case 4: // control
                schedInfo[i].N = 1;
                schedInfo[i].n = 0;
                break;

            case 5: // side lights
                schedInfo[i].N = 1000;
                schedInfo[i].n = 0;
                break;
        }
    }
}

void task_motors(){
   
   /*int dc1 = (int)(100*(surge + yaw));
   int dc2 = dc1;
   int dc3 = (int)(100*(surge - yaw));
   int dc4 = dc3;
   */ 
     
   //sprintf(tmp_buffer,"$MPWM,%d,%d,%d,%d*",dc1,dc2,dc3,dc4);
   sprintf(tmp_buffer,"$MPWM,%d,%d,%d,%d*",ocr_data.oc1r, ocr_data.oc2r, ocr_data.oc3r, ocr_data.oc4r);
   write_ringTX(tmp_buffer);
   
   write_uart(strlen(tmp_buffer));
}

void task_IR(){
    
    float volt;
    
    if(AD1CON1bits.DONE){
        // IR sensor output
        volt = ((float)ADC1BUF1/(float)(MAX_INT_ADC)) * VOLT_REF_IR;
        
        // distance in meter
        dist = 2.34 - 4.74 * volt + 4.06 * powf(volt,2) - 1.60 * powf(volt,3) + 0.24 * powf(volt,4);
        
        // print distance in cm
        sprintf(tmp_buffer,"$MDIST,%d*",(int)(dist*100));
        write_ringTX(tmp_buffer);
        
        write_uart(strlen(tmp_buffer));
        
        
    }
}

/*void task_sendUART(){
    
    while(U1STAbits.UTXBF == 0 && data_TX.head != data_TX.tail)
        U1TXREG = read_ringTX();
    //LATAbits.LATA0 = !LATAbits.LATA0;
}*/

void task_threshold(){
    
    char data_rcv;
    int ret, threshold_min, threshold_max; 
    
    
    while(data_RX.head != data_RX.tail){
            
            data_rcv = read_ringRX();
            ret = parse_byte(&pstate, data_rcv);
            if (ret == NEW_MESSAGE){
                if (strcmp(pstate.msg_type, MSG_INFO) == 0) {
                    sscanf(pstate.msg_payload,"%d,%d",&threshold_min,&threshold_max);
                    val_min = (double)threshold_min/100.0;
                    val_max = (double)threshold_max/100.0;
                    write_ringTX(ack_rcv);
                    
                } else if (ret == ERR_MESSAGE)
                    write_ringTX(ack_err);
        }
    }
    
   
}

void task_battery(){
    
   float volt_battery;
      
   volt_battery = ((float)ADC1BUF0/(float)(MAX_INT_ADC)) * VOLT_REF_BATTERY;
        //U1TXREG = printf("battery: %f", volt*3);
        //U1TXREG = printf("distance: %f and volt: %f", dist,volt*3);
    
   sprintf(tmp_buffer,"$MBATT,%.2f*",(double)(3*volt_battery));
   write_ringTX(tmp_buffer);
   write_uart(strlen(tmp_buffer));

}

void task_control(){
    /* control law
     * The control is expressed as sum between surge and yaw commands.
     * The buggy can turn only on the right, thus
     *      control_left = surge + yaw
     *      control_right = surge - yaw
     * By hypothesis surge and yaw included in [0,1]
     * 
     * if dist > MAXTH:
     *      surge = 1, yaw = 0, thus
     *      control_left = surge + yaw = 1
     *      control_right = surge - yaw = 1
     * if dist < MINTH:
     *      surge = 0, yaw = 1, thus
     *      control_left = surge + yaw = 1
     *      control_right = surge - yaw = -1
     * 
     * if MINTH < dist < MAXTH:
     *      proportional to the distance:
     *          surge = (dist - MINTH) / (MAXTH - MINTH)
     *          yaw = (MAXTH - dist) / (MAXTH - MINTH)
     *      thus
     *          control_left = surge + yaw = ((dist - MINTH) + (MAXTH - dist)) / (MAXTH - MINTH)
     *       => control_left = 1
     *          control_right = surge - yaw = ((dist - MINTH) - (MAXTH - dist)) / (MAXTH - MINTH)
             => control_right = 2*(dist - MINTH) / (MAXTH - MINTH) - 1
     */
    
    if(state == WAIT_FOR_START){
        surge = 0;
        yaw = 0;
        shutdown_motors();
        //blink led A0 to do
    }
    
    else if(state == MOVING){
        
        if(dist > val_max){
            surge = 1;
            yaw = 0;
        }
            
        else if(dist <= val_max && dist >= val_min){
            surge = (dist - val_min) / (val_max - val_min);
            yaw = (val_max - dist) / (val_max - val_min);
        }
        
        else if(dist < val_min){
            surge = 0;
            yaw = 1;
        }
        
        // ADC commands
        move(surge + yaw, surge - yaw);
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
                    task_threshold();
                    break;
                case 4:
                    task_control();
                    head_rear_lights(state, surge, yaw);
                    break;
                case 5:
                    side_lights(state, surge, yaw);
                    break;
            }
            schedInfo[i].n = 0;
        }
    }
    
}

#endif	/* XC_HEADER_TEMPLATE_H */

