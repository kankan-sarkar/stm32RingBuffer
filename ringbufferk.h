/*
 * ringbufferk.h
 *
 *  Created on: Jun 15, 2021
 *      Author: kanka
 */

#ifndef INC_RINGBUFFERK_H_
#define INC_RINGBUFFERK_H_

/* Inlcude your respective STM32 HAL Library*/
#include "stm32f0xx_hal.h"

// Define Here what should be the buffer size of the ring buffer
#define UART_BUFFER_SIZE 64

#if (UART_BUFFER_SIZE > 256)
typedef uint16_t rxbuffer_Index_Type;
typedef uint16_t txbuffer_Index_Type;
#else
typedef uint8_t rxbuffer_Index_Type;
typedef uint8_t txbuffer_Index_Type;
#endif

typedef struct{
	volatile uint8_t _buffer[UART_BUFFER_SIZE];
	volatile unsigned int _head;
	volatile unsigned int _tail;
	UART_HandleTypeDef *uart;
}RingBuffer;
uint8_t _ReceivedData;
RingBuffer RxBuffer,TxBuffer;


void __push(RingBuffer *buff,uint8_t a);
uint8_t __pop(RingBuffer *buff);




void Serial_begin(UART_HandleTypeDef *);
uint8_t Serial_available();
uint8_t Serial_flush();
uint8_t Serial_read();
void Serial_write(uint8_t c);
void Serial_print(const char *s);
void Serial_println(const char *s);
void Serial_find();
void Serial_findUntil();
void Serial_peek();
void Serial_readString();
#endif /* INC_RINGBUFFERK_H_ */
