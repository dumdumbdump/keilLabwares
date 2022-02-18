// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// March 13, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
unsigned char Index=0; // Index varies from 0 to 15
unsigned int isSilent = 0;
void DAC_Init(void){
    unsigned long volatile delay;
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B delay = SYSCTL_RCGC2_R; // allow time to finish activating GPIO_PORTB_AMSEL_R &= ~0x07; // no analog
 delay = SYSCTL_RCGC2_R; // allow time to finish activating
 GPIO_PORTB_DR8R_R |= 0x0F;
 GPIO_PORTB_AMSEL_R &= ~0x0F; // no analog
 GPIO_PORTB_PCTL_R &= ~0x00000FFF; // regular GPIO function GPIO_PORTB_DIR_R |= 0x07; // make PB2-0 out
 GPIO_PORTB_DIR_R |= 0x0F; // make PB2-0 out
 GPIO_PORTB_AFSEL_R &= ~0x0F; // disable alt funct on PB2-0
 GPIO_PORTB_DEN_R |= 0x0F; // enable digital I/O on PB2-0

}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){ 
    if(!isSilent)
        GPIO_PORTB_DATA_R = data;
    else {
        GPIO_PORTB_DATA_R = 0x00;
    }
}