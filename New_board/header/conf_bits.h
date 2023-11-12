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
#ifndef XC_CONF_BITS_H
#define	XC_CONF_BITS_H

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

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

