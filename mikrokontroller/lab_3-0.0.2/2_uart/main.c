#include "uart.h"
#include "gpio.h"
#include <stdio.h>
#include <sys/types.h>

#define UART_INT_RX_pin 8
#define UART_INT_TX_pin 6

#define BTN_1 13
#define BTN_2 14

ssize_t _write(int fd, const void *buf, size_t count) {
    char* letter = (char *)(buf);
    for(int i = 0; i< count; i++){
        uart_send(*letter);
        letter++;
        int sleep = 10000;
        while(--sleep);
    }
    return count;
}



int main() {
    for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTSET = (1 << i); 
	}


    GPIO->PIN_CNF[13] = (3 << 2);
	GPIO->PIN_CNF[14] = (3 << 2);
	GPIO->PIN_CNF[15] = (3 << 2);
	GPIO->PIN_CNF[16] = (3 << 2);

    uart_init();

    int sleep = 0;
    char c;
    while(1){
        if(!(GPIO->IN & (1 << 13))){
            uart_send('A');
            while(!(GPIO->IN & (1 << 13)));
        }
        if(!(GPIO->IN & (1 << 14))){
            uart_send('B');
            while(!(GPIO->IN & (1 << 14)));
        }
        if(!(GPIO->IN & (1 << 15))){
            iprintf("The average grade in TTK%d in %d was: %c\n\r", 4235, 2022, 'B');
            while(!(GPIO->IN & (1 << 15)));
        }



        c = uart_read();
        switch(c){
            case 'A':
                GPIO->OUTCLR = (1 << 17);
                GPIO->OUTCLR = (1 << 18);
                GPIO->OUTCLR = (1 << 19);
                GPIO->OUTCLR = (1 << 20);
                break;
            case 'B':
                GPIO->OUTSET = (1 << 17);
                GPIO->OUTSET = (1 << 18);
                GPIO->OUTSET = (1 << 19);
                GPIO->OUTSET = (1 << 20);
                break;
            default:
                break;
        }



        sleep = 10000;
        while(--sleep);
    }
    return 0;
}