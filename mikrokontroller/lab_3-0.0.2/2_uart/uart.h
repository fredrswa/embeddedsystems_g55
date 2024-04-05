#pragma once

#include <stdint.h>

void uart_init();
void uart_send(char letters);
char uart_read();


void uart_send_str(char ** str);