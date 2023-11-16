/*
 * File:   main.c
 * Author: Luca Petruzzello (s5673449), Carmine Miceli (s5626492), Davide Cattin (s5544178)
 *
 * Created on 24 ottobre 2023
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/config_bits.h"
#include "header/timer_utils.h"
#include "header/SPI_utils.h"
#include "header/UART_utils.h"
#include "header/button_utils.h"
#include "header/buffer_utils.h"
#include "header/interrupt_handle.h"


void algorithm(){
    
    // busy-waiting
    tmr_wait_ms(TIMER2, 7);
    
}

int main(void) {
    
    // initialization for the components of the program
    init_ring_buffer();
    init_SPI();
    init_button_S5();
    init_button_S6();
    init_UART();
    
    char data_to_display, number_char_received[4];
    int i, j = 0;
    
    // cycle-period of the task
    tmr_setup_period(TIMER1, TIME_PERIOD);
    // timer for antibouncing
    tmr_setup_period(TIMER3, TIME_BOUNCE);
    
    
    
    while(1){
        // busy work
        algorithm();
        
        // button S5 has been pressed
        if(state == STATE_S5){
            // send number of received characters
            sprintf(number_char_received, "%d", char_counter);
            while(number_char_received[j] != '\0'){
                write_UART(number_char_received[j]);
                j++;
            }
            j = 0;
            state = 0;
        }
        // button S6 has been pressed
        else if(state == STATE_S6){
            // reset LCD and variables related to it
            clear_SPI_row(SECOND_ROW);
            clear_SPI_row(FIRST_ROW);
            char_counter = 0;
            count_char_SPI = 0;
            state = 0;
        }
        
        // if new data have been received, write it to the LCD
        for(i=0; i<data_r.counter && data_r.head != data_r.tail; i++){
            
            data_to_display = read_ring();
            // check if special characters have been received
            if(data_to_display == (char)LF || data_to_display == (char)CR){
                clear_SPI_row(FIRST_ROW);
                count_char_SPI = 0;
            }

            else{
                // handle LCD overflow
                check_SPI_first_row_length();
                send_SPI_char(data_to_display);
            }
        }
        // update number of characters received with a message on the LCD
        create_send_string_second_row();
              
        // remaining time for the task
        tmr_wait_period(TIMER1);
    }
    return 0;
}
