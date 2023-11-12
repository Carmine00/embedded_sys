/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 9 novembre 2023, 11.03
 */


#include "xc.h"

// FGS
#pragma config GWRP = OFF // General Segment Write?Protect bit (General Segment may be written)
#pragma config GSS = OFF // General Segment Code?Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)
// FOSCSEL (Primary Oscillator, 8MHz quartz on pin OSC1)
#pragma config FNOSC = PRIPLL
#pragma config IESO = ON
// FOSC (Primary)
#pragma config POSCMD = XT // 3 to 10 MHz
#pragma config OSCIOFNC = OFF
#pragma config IOL1WAY = ON
#pragma config FCKSM = CSDCMD
// FWDT
#pragma config WDTPOST = PS32768 // Watchdog Timer Postscaler Bits (1:32,768)
#pragma config WDTPRE = PR128 // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid .)
#pragma config WINDIS = OFF // Watchdog Timer Window Enable bit (Watchdog Timer in Non?Window mode)
#pragma config FWDTEN = OFF // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)
// FPOR
#pragma config FPWRT = PWR128 // Power?on Reset Timer Value Select bits (128ms)
#pragma config BOREN = ON // Brown?out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = OFF // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = OFF // Alternate I2C pins for I2C2 (SDA2/SCK2 pins are selected as the I/O pins for I2C2)
// FICD
#pragma config ICS = PGD1 // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config RSTPRI = PF // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF // JTAG Enable bit (JTAG is disabled)
// FAS
#pragma config AWRP = OFF // Auxiliary Segment Write?protect bit (Auxiliary program memory is not write?protected)
#pragma config APL = OFF // Auxiliary Segment Code?protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)

int main(void) {
    
    // all analog pins are set to digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // set led as output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    
     LATAbits.LATA0 = 1;
   
    
    while(1){
       
    }
    
    
    return 0;
}
