/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 13 novembre 2023
 */


#include <xc.h>
#include "../../header/timer_utils.h"
#define UART_USE 1
#define BAUD 38400

// function to perform input/output remapping and configuration based on the UART used 
void UART_config(int val){
    
    switch(val){
        case 1:
            RPOR0bits.RP64R = 0x01; // Map UART 1 TX to pin RD0 which is RP64
            RPINR18bits.U1RXR = 0x4B; // Map UART 1 RX to pin RD11 which is RPI75 (0x4B = 75)
            U1BRG = (long)F_CY/(16L * (long)BAUD)-1; 
            U1MODEbits.UARTEN = 1; // enable UART
            U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
            U1STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
            U1STAbits.URXISEL = 0; // generate interrupt everytime a character is received
            IEC0bits.U1RXIE = 1; // enable uart 1 RX intterupt
            break;
        case 2:
            RPOR6bits.RP85R = 0x03; // Map UART 2 TX to pin RE5 which is RP85
            RPINR19bits.U2RXR = 0x56; // Map UART 2 RX to pin RE6 which is RPI86 (0x56 = 86)
            U2BRG = (long)F_CY/(16L * (long)BAUD)-1; // = 11
            U2MODEbits.UARTEN = 1; // enable UART
            U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
            U2STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
            U2STAbits.URXISEL = 0; // generate interrupt everytime a character is received
            IEC1bits.U2RXIE = 1; // enable uart 2 RX intterupt
    }
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _U1RXInterrupt() {
    IFS0bits.U1RXIF = 0; // reset interrupt flag
    
    U1TXREG = U1RXREG;
    
    if(U1RXREG == 'a')
        LATAbits.LATA0 = 1;
    else if (U1RXREG == 'g')
        LATGbits.LATG9 = 1;
    else if (U1RXREG == 't'){
        LATAbits.LATA0 = 0;
        LATGbits.LATG9 = 0;
    }
}

int main(void) {
    
    // all pins set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // set led RA0 as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    // set led RG9 as output
    TRISGbits.TRISG9 = 0;
    LATGbits.LATG9 = 0;
    
    UART_config(UART_USE);
    
    U1TXREG = 'c';
    
    while(1){}
    
    
    return 0;
}
