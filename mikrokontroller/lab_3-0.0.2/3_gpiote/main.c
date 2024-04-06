#include "gpiote.h"
#include "gpio.h"
#include "ppi.h"


#define BTN_A 13

#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20


void init();

int main() {
    for(int i = 17; i <= 20; i++) {
        GPIO->DIRSET = (1 << i);
        GPIO->OUTSET = (1 << i);
    }
    /**Configure GPIOTE
     * Configure ch 0 to generate event on button A press
     * Configure ch 1 to toggle LED 1 on event from ch 0
     * Configure ch 2 to toggle LED 2 on event from ch 0
     * Configure ch 3 to toggle LED 3 on event from ch 0
     * Configure ch 4 to toggle LED 4 on event from ch 0
    */

    //BTN_A
    GPIOTE->CONFIG[0] |= (1 << 0)|(BTN_A << 8)|(2 << 16);

    //LED_1 to LED_4
    GPIOTE->CONFIG[1] = (3 << 0)|(LED_1 << 8)|(3 << 16)|(1 << 20);
    GPIOTE->CONFIG[2] = (3 << 0)|(LED_2 << 8)|(3 << 16);
    GPIOTE->CONFIG[3] = (3 << 0)|(LED_3 << 8)|(3 << 16);
    GPIOTE->CONFIG[4] = (3 << 0)|(LED_4 << 8)|(3 << 16)|(1 << 20);

    //Configure PPI
    PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[1]);
    PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[2]);
    PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[3]);
    PPI->PPI_CH[3].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[3].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[4]);

    for(int i = 0; i < 4; i++) {
        PPI->CHENSET = (1 << i);
    }




    while(1);
    return 0;
}




void init() {
    for(int i = 17; i <= 20; i++) {
        GPIO->DIRSET = (1 << i);
        GPIO->OUTCLR = (1 << i);
    }
    int sleep = 0;
    for(int i = 17; i <= 20; i++) {
        sleep = 1000000;
        while(--sleep);
        GPIO->OUTSET = (1 << i);
    }
    for(int i = 17; i <= 20; i++) {
        sleep = 1000000;
        while(--sleep);
        GPIO->OUTCLR = (1 << i);

    }
    for(int i = 17; i <= 20; i++) {
        sleep = 1000000;
        while(--sleep);
        GPIO->OUTSET = (1 << i);
    }
    sleep = 1000000;
    while(--sleep);
}