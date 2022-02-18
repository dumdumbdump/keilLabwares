// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano
// September 30, 2013

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "..//tm4c123gh6pm.h"

unsigned long data;
// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void){ unsigned long volatile delay;
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // activate port B delay = SYSCTL_RCGC2_R; 
 delay = SYSCTL_RCGC2_R; // allow time to finish activating
 GPIO_PORTE_AMSEL_R &= ~0x0F; // no analog
 GPIO_PORTE_PCTL_R &= ~0x00000FFF; // regular GPIO function 
 GPIO_PORTE_DIR_R &= ~0x0F; // make PB2-0 out
 GPIO_PORTE_AFSEL_R &= ~0x0F; // disable alt funct on PB2-0
 GPIO_PORTE_DEN_R |= 0x0F; // enable digital I/O on PB2-0
}
// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
  data = GPIO_PORTB_DATA_R;
  if(data &0x08)
    return 4;
  else if(data & 0x04)
    return 3;
  else if(data & 0x02)
    return 2;
  else if(data & 0x01)
    return 1; 
  else 
    return 0;
}
