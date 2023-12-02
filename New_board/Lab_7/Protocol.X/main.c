/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 2 dicembre 2023
 */


#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "../../header/uart_utils.h"
#include "../../header/buffer_utils.h"
#include "../../header/protocol_utils.h"
#define UART_USE 1
char ack[] = "OK";
char ack_err[] = "ERR";

void __attribute__ ((__interrupt__ , __auto_psv__)) _U1RXInterrupt() {
    IFS0bits.U1RXIF = 0; // reset interrupt flag
    
    write_ring((char)U1RXREG); 
}

void write_uart(int val){
    
    int i = 0;
    
    switch(val){
        case 1:
            while(ack[i] != '\0'){
                U1TXREG = ack[i];
                i++;
            }
        break;
        case -1:
            while(ack_err[i] != '\0'){
                U1TXREG = ack_err[i];
                i++;
            }
        break;
    }
}

int main(void) {
    
    // all pins set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    init_ring_buffer();
    // single byte received from uart
    char data_rcv;
    int ret, val; 
    parser_state pstate;
    pstate.state = STATE_DOLLAR;
    pstate.index_type = 0;
    pstate.index_payload = 0;

    
    // set led RA0 as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    // set led RG9 as output
    TRISGbits.TRISG9 = 0;
    LATGbits.LATG9 = 0;
    
    UART_config(UART_USE);
    
    
    while(1){       
        if(data_r.head != data_r.tail){
            
            data_rcv = read_ring();
            ret = parse_byte(&pstate, data_rcv);
            if (ret == NEW_MESSAGE){
                if (strcmp(pstate.msg_type, "L1") == 0) {
                    write_uart(NEW_MESSAGE);
                    sscanf(pstate.msg_payload,"%d",&val);
                    switch(val){
                        case 0:
                            LATAbits.LATA0 = 0;
                        break;
                        case 1:
                            LATAbits.LATA0 = 1;
                        break;
                    }
                } else if (strcmp(pstate.msg_type, "L2") == 0) {
                    write_uart(NEW_MESSAGE);
                    sscanf(pstate.msg_payload,"%d",&val);
                    switch(val){
                        case 0:
                            LATGbits.LATG9 = 0;
                        break;
                        case 1:
                            LATGbits.LATG9 = 1;
                        break;
                    }
                }
            } else if (ret == ERR_MESSAGE)
                write_uart(ERR_MESSAGE);
        }
    
    }
    
    
    return 0;
}
