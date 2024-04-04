#include <stdint.h>

#define GPIO ((NRF_GPIO_REGS*)0x50000000)

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t RESERVED1[118];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;


#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20
#define BTN_1 13
#define BTN_2 14
#define BTN_3 15
#define BTN_4 16



int main(){
	// Configure LED Matrix
	for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTSET = (1 << i); 
	}

	// Configure buttons
	GPIO->PIN_CNF[13] = (3 << 2);
	GPIO->PIN_CNF[14] = (3 << 2);
	GPIO->PIN_CNF[15] = (3 << 2);
	GPIO->PIN_CNF[16] = (3 << 2);


	int sleep = 0;
	int sleeptime = 10000;
	while(1){		

		if(!(GPIO->IN & (1 << BTN_1))){
			GPIO->OUTCLR = (1 << LED_1);
			GPIO->OUTCLR = (1 << LED_2);
			GPIO->OUTCLR = (1 << LED_3);
			GPIO->OUTCLR = (1 << LED_4);
		}

		if(!(GPIO->IN & (1 << BTN_2))){
			GPIO->OUTSET = (1 << LED_1);
			GPIO->OUTSET = (1 << LED_2);
			GPIO->OUTSET = (1 << LED_3);
			GPIO->OUTSET = (1 << LED_4);
		}
		sleep = sleeptime;
		while(--sleep); // Delay

		
	}
	return 0;
}
