#ifndef PPI_H
#define PPI_H
#include <stdint.h>

#define PPI ((NRF_PPI_REG*)0x4001F000)

typedef struct {
	uint32_t EN;
	uint32_t DIS;
} PPI_TASKS_TYPE;

typedef struct {
	uint32_t EEP;
	uint32_t TEP;
} PPI_CHANNEL_TYPE;

typedef struct {
	// Tasks
	volatile PPI_TASKS_TYPE PPI_TASKS[12];
	// Registers
	volatile uint32_t RESERVED0[308];
	volatile uint32_t CHEN;
	volatile uint32_t CHENSET;
	volatile uint16_t CHENCLR;
	volatile PPI_CHANNEL_TYPE PPI_CH[40];
	volatile uint32_t RESERVED2[148];
	volatile uint32_t CHG[6];
	volatile uint32_t RESERVED3[62];
	volatile PPI_CHANNEL_TYPE FORK[32];
} NRF_PPI_REG;

#endif
