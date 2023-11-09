/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 19 ottobre 2023
 */

#define F_CLK 7372800
#define F_CY F_CLK/4 //1.843.200 Hz
#define MAX_INT 65535
#define TIMER1 1
#define TIMER2 2
#define LEN 8
#include <xc.h>
#include <stdio.h>
#include <string.h>
// FOSC
#pragma config FPR = XT // Primary Oscillator Mode (XT)
#pragma config FOS = PRI // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF // Clock Switching and Monitor (Sw Disabled, Mon Disabled)
// FWDT
#pragma config FWPSB = WDTPSB_16 // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512 // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF // Watchdog Timer (Disabled)
// FBORPOR
#pragma config FPWRT = PWRT_64 // POR Timer Value (64ms)
#pragma config BODENV = BORV20 // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI // Low?side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI // High?side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN // Master Clear Enable (Enabled)
// FGS
#pragma config GWRP = GWRP_OFF // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF // General Segment Code Protection (Disabled)
// FICD
#pragma config ICS = ICS_PGD // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

int counter = -1; // counter for the seconds elapsed
char str[LEN]; // string containing the seconds to be printed

void tmr_setup_period(int timer, int ms) {
    
    // set prescale
    float prescale_f = (float)ms / 1000.0 * F_CY / (float)MAX_INT;
    int prescale, prescale_binary;
    
    
    if (prescale_f <= 1){
        prescale = 1;
        prescale_binary = 0;
    }
    else if (prescale_f <= 8){
        prescale = 8;
        prescale_binary = 1;
    }
    else if (prescale_f <= 64){
        prescale = 64;
        prescale_binary = 2;
    }
    else if (prescale_f <= 256){
        prescale = 256;
        prescale_binary = 3;
    }
    else
        ; // error to handle
    
    switch(timer){
        case TIMER1:
            // reset timer
            TMR1 = 0;
            
            // set period register
            PR1 = (uint16_t)((float)ms / 1000.0 * F_CY / (float)prescale);
            
            // configure timer
            T1CONbits.TON = 0;  // disable timer
            T1CONbits.TCKPS = prescale_binary; // set prescale
            T1CONbits.TCS = 0;  // external clock
            T1CONbits.TGATE = 0;
            
            // reset interrupt timer 1
            IFS0bits.T1IF = 0;
            
            T1CONbits.TON = 1;
            break;
        
        case TIMER2:
            // reset timer
            TMR2 = 0;
            
            // set period register
            PR2 = (uint16_t)((float)ms / 1000.0 * F_CY / (float)prescale);
            
            // configure timer
            T2CONbits.TON = 0;  // disable timer
            T2CONbits.TCKPS = prescale_binary; // set prescale
            T2CONbits.TCS = 0;  // external clock
            T2CONbits.TGATE = 0;
            T2CONbits.T32 = 0;
            
            // reset interrupt timer 2
            IFS0bits.T2IF = 0;
            
            T2CONbits.TON = 1;
            break;
    }
}

void tmr_wait_period(int timer){
    switch(timer){
        case TIMER1:
            if (IFS0bits.T1IF == 1){
                LATBbits.LATB1 = 1;
            }
            while (IFS0bits.T1IF == 0){}
            IFS0bits.T1IF = 0;
            break;
            
        case TIMER2:
            if (IFS0bits.T2IF == 1){
                LATBbits.LATB1 = 1;
            }
            while (IFS0bits.T2IF == 0) {}
            IFS0bits.T2IF = 0;
            break;
    }
}

void tmr_wait_ms(int timer, int ms) {
    tmr_setup_period(timer,ms);
    tmr_wait_period(timer);
}

void reset_LCD(){
    
    SPI1BUF = 0x80;
    
    for(int i = 0; i < LEN; i++){
        while(SPI1STATbits.SPITBF == 1); // when condition == 0, buffer empty
        SPI1BUF = ' ';
    }
    
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _T1Interrupt() {
    IFS0bits.T1IF = 0; // reset interrupt flag
    counter++;
    sprintf(str, "%d",counter);
    SPI1BUF = 0x80; // write in the first row, first column

    for(int i = 0; i < strlen(str); i++){
        while(SPI1STATbits.SPITBF == 1); // when condition == 0, buffer empty
        SPI1BUF = str[i];
    }   
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT0Interrupt() {
    // INT0 is related to an external interrupt, in this case the click of button S5 (E8)
    //IFS0bits.INT0IF = 0; // reset interrupt flag
    IEC0bits.INT0IE = 0; // disable interrupt button
    T2CONbits.TON = 1;
}

// interrupt for button bouncing
void __attribute__ ((__interrupt__ , __auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0;        
    // check button status
    if(PORTEbits.RE8 != 0){
        counter = 0;
        reset_LCD();
    }
    T2CONbits.TON = 0; // disable timer
    TMR2 = 0; // reset counter for the timer because we don't know when this interrupt happens
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 1; // able interrupt firing for button
}

int main(void) {
    
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8 bit mode
    SPI1CONbits.PPRE = 2; // 4:1 primary prescaler
    SPI1CONbits.SPRE = 7; // 1:1 secondary prescaler --> 111; freq_SPI = 460.800 Hz
    SPI1STATbits.SPIEN = 1; // enable SPI
    IEC0bits.T1IE = 1; // enable interrupt TIMER1
    IEC0bits.T2IE = 1; // enable interrupt TIMER2
    IEC0bits.INT0IE = 1; // enable interrupt button S5
    
    TRISEbits.TRISE8 = 1; // set E8 (switch S5) as input pin
    
    tmr_setup_period(TIMER2,20);


    while(1)
        tmr_wait_ms(TIMER1, 1000);
    return 0;
}
