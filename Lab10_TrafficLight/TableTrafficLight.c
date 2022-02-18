// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// November 7, 2013

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****



// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****

struct State {
  unsigned int nextStates[8];
  unsigned int ledValues[8];
  unsigned int delay;
  unsigned long PortF_Out;
  unsigned long PortB_Out;
};
unsigned long current_state;
unsigned long nextState;
unsigned long tmp1;
unsigned long tmp2;
struct State states[9] = {
  {{1,1,1,1,0,1,1,1},{0,0,1,1,0,0,0,1},10,0x02,0x0c},
  {{2,4,2,2,2,2,4,2},{0,1,0,1,0,0,0,1},10,0x02,0x14},
  {{3,3,2,3,3,3,3,3},{1,0,0,0,0,1,0,1},10,0x02,0x21},
  {{4,4,4,4,4,4,0,4},{1,0,0,0,1,0,0,1},10,0x02,0x22},
  {{5,4,5,5,5,5,5,5},{1,0,0,1,0,0,1,0},10,0x04,0x24},
  {{6,4,6,6,6,6,6,6},{1,0,0,1,0,0,0,1},5,0x02,0x24},
    {{7,4,7,7,7,7,7,7},{1,0,0,1,0,0,0,0},5,0x00,0x24},
  {{8,4,8,8,8,8,8,8},{1,0,0,1,0,0,0,1},5,0x02,0x24},
    {{0,4,2,2,0,0,0,0},{1,0,0,1,0,0,0,0},5,0x00,0x24}
};

void Ports_Init() { volatile unsigned long delay;
  	SYSCTL_RCGC2_R |= 0x32;               	    // 1) B E F  
	delay = SYSCTL_RCGC2_R;               	   
    // GPIO_PORTF_LOCK_R = 0x4C4F434B;
    // GPIO_PORTB_LOCK_R = 0x4C4F434B;
    // GPIO_PORTE_LOCK_R = 0x4C4F434B;

	GPIO_PORTE_AMSEL_R    &= ~0x07;           	// 3) disable analog function on PE2-0  
	GPIO_PORTE_PCTL_R     &= ~0x00000FFF; 		// 4) enable regular GPIO  
	GPIO_PORTE_DIR_R      &= ~0x07;       		// 5) inputs on PE2-0  
	GPIO_PORTE_AFSEL_R    &= ~0x07;         	// 6) regular function on PE2-0  
	GPIO_PORTE_DEN_R      |=  0x07;         	// 7) enable digital on PE2-0  
  
	GPIO_PORTB_AMSEL_R    &= ~0x3F;           	// 3) disable analog function on PB5-0  
	GPIO_PORTB_PCTL_R     &= ~0x00FFFFFF; 		// 4) enable regular GPIO  
	GPIO_PORTB_DIR_R      |=  0x3F;           	// 5) outputs on PB5-0  
	GPIO_PORTB_AFSEL_R    &= ~0x3F;           	// 6) regular function on PB5-0  
	GPIO_PORTB_DEN_R      |=  0x3F;           	// 7) enable digital on PB5-0  
  
	GPIO_PORTF_AMSEL_R    &= ~0x0A;       		// 3) disable analog on PF1, PF3  
	GPIO_PORTF_PCTL_R     &= ~0x0000F0F0; 		// 4) PCTL GPIO on PF1, PF3  
	GPIO_PORTF_DIR_R      |=  0x0A;       		// 5) PF3, PF1 out  
	GPIO_PORTF_AFSEL_R    &= ~0x0A;       		// 6) disable alt funct on PF1, PF3  
	GPIO_PORTF_DEN_R      |=  0x0A;       		// 7) enable digital I/O on PF1, PF3  
}

void SysTick_Init(void){  
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup  
	NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock  
}  
  
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)  
void SysTick_Wait(unsigned long delay)  
{  
	NVIC_ST_RELOAD_R = delay-1; 			// number of counts to wait  
	NVIC_ST_CURRENT_R = 0;       			// any value written to CURRENT clears  
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // wait for count flag  
}  
  
// 800000*12.5ns equals 10ms  
void SysTick_Wait10ms(unsigned long delay)
{  
	unsigned long i;  
	for(i=0; i<delay; i++)
		SysTick_Wait(800000);  // wait 10ms  
}

void controlLEDs(){
  GPIO_PORTB_DATA_R = states[current_state].PortB_Out;
  GPIO_PORTF_DATA_R = states[current_state].PortF_Out;
} //according to the current state to change LEDs 
int main(void){ 
TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz
SysTick_Init();
Ports_Init();
current_state = 0;
EnableInterrupts();
while(1){
    controlLEDs();
    SysTick_Wait10ms(states[current_state].delay);
    nextState = GPIO_PORTE_DATA_R;
    tmp1 = nextState & 0x01;
    tmp1 = tmp1<<2;
    tmp2 = nextState & 0x04;
    tmp2 = tmp2>>2;
    if(tmp1) {
      nextState = nextState | tmp1;
    }
    else {
      nextState = nextState & tmp1;
    }
    if(tmp2) {
      nextState = nextState | tmp2;
    }
    else {
      nextState = nextState & tmp2;
    }
    current_state = states[current_state].nextStates[nextState];
  }
  return 0;
}

