/*
 * File:   main.c
 * Author: Carmine
 *
 * Created on 9 novembre 2023, 11.03
 */


#include "xc.h"

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
