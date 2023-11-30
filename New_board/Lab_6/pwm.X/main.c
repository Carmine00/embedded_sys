/*
 * File:   main.c
 * Author: lucap
 *
 * Created on 30 novembre 2023, 11.54
 */

#define FOSC 144000000
#define FCY FOSC/2
#include "xc.h"

int main(void) {
    
    OC1CON1bits.OCTSEL = 7;    // perpheral clock used as source for output compare
    OC1CON2bits.SYNCSEL = 0x1F; // no sync required
    OC1CON1bits.OCM = 6;       // edge align PWM
    
    OC2CON1bits.OCTSEL = 7;    // perpheral clock used as source for output compare
    OC2CON2bits.SYNCSEL = 0x1F; // no sync required
    OC2CON1bits.OCM = 6;       // edge align PWM
    
    OC3CON1bits.OCTSEL = 7;    // perpheral clock used as source for output compare
    OC3CON2bits.SYNCSEL = 0x1F; // no sync required
    OC3CON1bits.OCM = 6;       // edge align PWM
    
    OC4CON1bits.OCTSEL = 7;    // perpheral clock used as source for output compare
    OC4CON2bits.SYNCSEL = 0x1F; // no sync required
    OC4CON1bits.OCM = 6;       // edge align PWM
    
    OC1R = 0;
    OC2R = 0;
    OC3R = 0;
    OC4R = 0;              // Duty cycle = Max PTPER
    OC1RS = OC2RS = OC3RS = OC4RS = 60;             // pwm period = PTPER
    
    RPOR0bits.RP65R = 0x10;  // mapping pin out rd1-rd2-rd3-rd4
    RPOR1bits.RP66R = 0x11;
    RPOR1bits.RP67R = 0x12;
    RPOR2bits.RP68R = 0x13;
    
    while(1);
      
    
    return 0;
}
