#ifndef GPIOTE_H
#define GPIOTE_H
#include <stdint.h>

#define GPIOTE ((NRF_GPIOTE_REG*)0x40006000)

typedef struct {
	// Tasks
	volatile uint32_t OUT[7];
	volatile uint32_t RESERVED0[4];
	volatile uint32_t SET[7];
	volatile uint32_t RESERVED1[4];
	volatile uint32_t CLR[7];
	volatile uint32_t RESERVED2[32];
	// Events
	volatile uint32_t IN[7];
	volatile uint32_t RESERVED3[23];
	volatile uint32_t PORT;
	// Registers
	volatile uint32_t RESERVED4[97];
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
	volatile uint32_t RESERVED5[129];
	volatile uint32_t CONFIG[7];
} NRF_GPIOTE_REG;

#endif
