/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 24 ottobre 2023
 */

#define F_CLK 7372800
#define F_CY F_CLK/4 //1.843.200 Hz
#define BAUD 9600
#define MAX_INT 65535
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define SPI_MAX_ROW_CHAR 16
#define LF 0x0A
#define CR 0x0D
#define FIRST_ROW 0x80
#define SECOND_ROW 0xC0
#define BUFFER 16
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
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



/******************************************************************************************************************
 * IMPLEMENTAZIONE RING BUFFER
 ******************************************************************************************************************/

typedef struct{
    //char* buf;
    char buf[BUFFER];
    int head;
    int tail;
    int counter;
} RingBuffer;

RingBuffer data_r;
char data_uart;
int count_char_SPI = 0, char_counter = 0, state_button;

void init_ring_buffer(){
    
    //data_r.buf = (char*)malloc(BUFFER * sizeof(char));
    data_r.head = 0;
    data_r.tail = 0;
    data_r.counter = 0;
}

void write_ring(char data){
    
    data_r.buf[data_r.head] = data;
    data_r.head = (data_r.head + 1) % BUFFER;
    data_r.counter++;
    
    //send_SPI_char(data_r.buf[data_r.head-1]);
}

char read_ring(){
    
    char data;
    
    data = data_r.buf[data_r.tail];
    data_r.tail = (data_r.tail + 1) % BUFFER;
    
    return data;
}

void reset_ring(){
    
    data_r.head = 0;
    data_r.tail = 0;
    data_r.counter = 0;
}

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
            
            case TIMER3:
            // reset timer
            TMR3 = 0;
            
            // set period register
            PR3 = (uint16_t)((float)ms / 1000.0 * F_CY / (float)prescale);
            
            // configure timer
            T3CONbits.TON = 0;  // disable timer
            T3CONbits.TCKPS = prescale_binary; // set prescale
            T3CONbits.TCS = 0;  // external clock
            T3CONbits.TGATE = 0;
            
            // reset interrupt timer 3
            IFS0bits.T3IF = 0;
            IEC0bits.T3IE = 1; // enable interrupt
            
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

void init_SPI(){
    
    tmr_wait_ms(TIMER1, 1000);
    // LCD configuration
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8 bit mode
    SPI1CONbits.PPRE = 2; // 4:1 primary prescaler
    SPI1CONbits.SPRE = 7; // 1:1 secondary prescaler --> 111; freq_SPI = 460.800 Hz
    SPI1STATbits.SPIEN = 1; // enable SPI
    
    
}

void init_UART(){
    
    // UART configuration
    
    IEC1bits.U2RXIE = 1; // enable interrupt
    
    U2BRG = F_CY/(16L * (long)BAUD)-1; // = 11
    U2MODEbits.STSEL = 0;     // 1 Stop bit
    U2MODEbits.PDSEL = 0;     // 8-bit data, no parity
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
    U2STAbits.URXISEL = 0; // generate interrupt everytime a character is received
    
}

void send_SPI_char(char data){
   
    //for(int i = 0; data[i] != '\0'; i++) {
        while(SPI1STATbits.SPITBF == 1);
        
        SPI1BUF = data;
    //}
}

char read_UART(){
    
    return (char)U2RXREG;
    
}


void write_UART(char c){
    
    U2TXREG = c;
    
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _U2RXInterrupt() {
    
    IFS1bits.U2RXIF = 0; // reset interrupt flag
    
    write_ring(read_UART());
    
    char_counter++;
    
    //send_SPI_char(read_ring());
    
    //write_UART(data_r);
}

void algorithm() {
    
    tmr_wait_ms(TIMER2, 7);
}

/*void clear_SPI_first_row(){
    
    send_SPI_char(FIRST_ROW);
    
    for(int i = 0; i < SPI_MAX_ROW_CHAR ; i++){
        while(SPI1STATbits.SPITBF == 1);
        SPI1BUF = ' ';
    }
    
    send_SPI_char(FIRST_ROW);
}*/

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
        //clear_SPI_first_row();
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

void init_button_S5(){
  
    
    IEC0bits.INT0IE = 1;
    TRISEbits.TRISE8 = 1;
}

void init_button_S6(){
    
    IEC1bits.INT1IE = 1;
    TRISDbits.TRISD0 = 1;
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT0Interrupt(){
    
    
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 0;
    T3CONbits.TON = 1;
    state_button = 5;
    
    
    
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _INT1Interrupt(){
    
    IFS1bits.INT1IF= 0;
    IEC1bits.INT1IE = 0;
    T3CONbits.TON = 1;
    state_button = 6;
    
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _T3Interrupt(){
    
    IFS0bits.T3IF = 0;
    
    int i = 0;
    char number_char_received[4];
    
    if(PORTEbits.RE8 && state_button == 5){
        sprintf(number_char_received, "%d", char_counter);
        while(number_char_received[i] != '\0'){
            write_UART(number_char_received[i]);
            i++;
        }
    }
    
    else if(PORTDbits.RD0 && state_button == 6){
        clear_SPI_row(SECOND_ROW);
        clear_SPI_row(FIRST_ROW);
        char_counter = 0;
        count_char_SPI = 0;
    }
    
    TMR3 = 0;
    T3CONbits.TON = 0;
    
    IEC0bits.INT0IE = 1;
    IEC1bits.INT1IE = 1;
    
    IFS0bits.INT0IF = 0;
    IFS1bits.INT1IF = 0;
}

int main(void) {
    
    ADPCFG = 1;
    
    TRISBbits.TRISB0 = 0;
    LATBbits.LATB0 = 0;
    
    //const char* message = "Hello, World";
    
    init_ring_buffer();
    init_SPI();
    init_button_S5();
    init_button_S6();
    init_UART();
    char data_to_display;
    int i;
    
    // cycle-period of the task
    tmr_setup_period(TIMER1, 10);
    // timer for antibouncing
    tmr_setup_period(TIMER3, 20);
    
    
    
    while(1){
        // busy work
        algorithm();
        
        //send_SPI_char(FIRST_ROW);
        
        for(i=0; i<data_r.counter && data_r.head != data_r.tail; i++){
            
            LATBbits.LATB0 = 1;
            
            data_to_display = read_ring();
            if(data_to_display == (char)LF || data_to_display == (char)CR){
                clear_SPI_row(FIRST_ROW);
                count_char_SPI = 1;
            }

            else{
                check_SPI_first_row_length();
                send_SPI_char(data_to_display);
                //write_UART(data_to_display);
            }

           
        }
        
        //clear_SPI_second_row();
        
        create_send_string_second_row();
        
        //reset_ring();        
        
        tmr_wait_period(TIMER1);
    }


    
    
    return 0;
}
