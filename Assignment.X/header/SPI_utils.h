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
#ifndef XC_SPI_UTILS_H
#define	XC_SPI_UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#define SPI_MAX_ROW_CHAR 16
#define LF 0x0A
#define CR 0x0D
#define FIRST_ROW 0x80
#define SECOND_ROW 0xC0
int count_char_SPI = 0, char_counter = 0;


void init_SPI(){
    
    tmr_wait_ms(TIMER1, 1000);
    // LCD configuration
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8 bit mode
    SPI1CONbits.PPRE = 2; // 4:1 primary prescaler
    SPI1CONbits.SPRE = 7; // 1:1 secondary prescaler --> 111; freq_SPI = 460.800 Hz
    SPI1STATbits.SPIEN = 1; // enable SPI
}

void send_SPI_char(char data){
    
    while(SPI1STATbits.SPITBF == 1);
        
    SPI1BUF = data;
}

void clear_SPI_row(int row){
    
    send_SPI_char(row);
    
    for(int i = 0; i < SPI_MAX_ROW_CHAR ; i++){
        while(SPI1STATbits.SPITBF == 1);
        SPI1BUF = ' ';
    }
    
    send_SPI_char(row);
}

void check_SPI_first_row_length(){
    
    count_char_SPI++;
    
    if(count_char_SPI > SPI_MAX_ROW_CHAR){
        clear_SPI_row(FIRST_ROW);
        count_char_SPI = 1;
    }
}

void create_send_string_second_row(){
    
    int i = 0;
    char char_recv[16];
    
    sprintf(char_recv, "Char Recv: %d", char_counter);
    
    send_SPI_char(SECOND_ROW);
    
    while(char_recv[i] != '\0'){
        send_SPI_char(char_recv[i]);
        i++;
    }
    
    send_SPI_char(FIRST_ROW+count_char_SPI);
}

#endif	/* XC_HEADER_TEMPLATE_H */

