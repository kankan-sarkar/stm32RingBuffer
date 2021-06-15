/*
 * ringbufferk.h
 *
 *  Created on: Jun 15, 2021
 *      Author: kanka
 */

#ifndef INC_RINGBUFFERK_H_
#define INC_RINGBUFFERK_H_

/* Inlcude your respective STM32 HAL Library*/
#include "stm32f4xx_hal.h"

// Define Here what should be the buffer size of the ring buffer
#define UART_BUFFER_SIZE 512


typedef struct{
	uint8_t _buffer[UART_BUFFER_SIZE];
	volatile unsigned int _head;
	volatile unsigned int _tail;
}RingBuffer;

void Serial_begin();
boolean Serial_available();
boolean Serial_flush();
uint8_t Serial_read();
void Serial_write(uint8_t _char);
void Serial_print(const char *s);
void Serial_println(const char *s);
void Serial_find();
void Serial_findUntil();
void Serial_peek();

#ifdef UART_BUFFER_SIZE > 255
uint8_t Serial_readBytes();
#else
uint16_t Serial_readBytes();
#endif

#endif /* INC_RINGBUFFERK_H_ */
