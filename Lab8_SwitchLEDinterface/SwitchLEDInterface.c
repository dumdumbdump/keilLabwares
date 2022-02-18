// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 8, 2014

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"
#include "tm4c123gh6pm.h"

#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// 2. Declarations Section
//   Global Variables
unsigned long In;  // input from PF4
unsigned long Out; // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void PortE_Init(void);
void Delay(void);
// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
unsigned long LED;

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	PortE_Init();
	GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R | 0x02;
  EnableInterrupts();           // enable interrupts for the grader
  
  while(1){
    // body goes here
		Delay();
		In = GPIO_PORTE_DATA_R&0x01; // read PF4 into In
		if(In == 0x01) {
			
			LED = GPIO_PORTE_DATA_R & 0x02;
			if(LED == 0x00)
				GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R | 0x02;
			else
				GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R & (~0x02);
			
		}
		else {
			GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R | 0x02;
		}
  }
}
// Subroutine to initialize port F pins for input and output
// PF4 and PF0 are input SW1 and SW2 respectively
// PF3,PF2,PF1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortE_Init(void){ 
volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay  	
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTE_CR_R = 0x03;           // allow changes to PE1-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x02;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
//   GPIO_PORTE_PUR_R = 0x11;          // enale pullup resistors on PF4,PF0       
  GPIO_PORTE_DEN_R = 0x03;          // 7) enable digital pins PF4-PF0        
}

void Delay(void){unsigned long volatile time;
  time = 727240*200/91*3/7;  // 
  while(time){
		time--;
  }
}
