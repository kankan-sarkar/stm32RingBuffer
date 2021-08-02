/*
 * ringbufferk.c
 *
 *  Created on: Jun 15, 2021
 *      Author: kanka
 */
#include "ringbuffer.h"

void Serial_begin(UART_HandleTypeDef *uart){
RxBuffer.uart=uart;
TxBuffer.uart=uart;
}

void __push(RingBuffer *buff,uint8_t a){
uint8_t i=(unsigned int)(buff->_head + 1) % UART_BUFFER_SIZE;
if(i!=buff->_tail){
	buff->_buffer[buff->_head]=a;
	buff->_head=i;
}
else{
//do nothing
}
}

uint8_t __pop(RingBuffer *buff){
	if(buff->_tail==buff->_head)
		{
			return 0;
		}
	else{
		uint8_t __data=buff->_buffer[buff->_tail];
		buff->_tail=(unsigned int)(buff->_tail + 1) % UART_BUFFER_SIZE;
		return __data;
	}
}

uint8_t Serial_available(RingBuffer *buff){
	 return ((unsigned int)(UART_BUFFER_SIZE + buff->_head - buff->_tail)) % UART_BUFFER_SIZE;
}

void Serial_write(uint8_t c){
__push(&TxBuffer,c);
__HAL_UART_ENABLE_IT(TxBuffer.uart,UART_IT_TXE);
}
void Serial_println(const char *datatosend){
	while(*datatosend) Serial_write((uint8_t)*datatosend++);
	Serial_write(0x0A);
//	Serial_write(0x00); // NULL
//	Serial_write(0x01); // CTRL+A Start of Header(SOH)
//	Serial_write(0x03); // CTRL+C End of Text (ETX)
//	Serial_write(0x04); // CTRL+D End of Transmission (EOT)
//	Serial_write(0x05); // CTRL+E Enquiry (SOH)
//	Serial_write(0x06); // CTRL+F Acknowledgement (ETX)
//	Serial_write(0x07); // CTRL+G BEL
//	Serial_write(0x0A); // New Line/LineFeed
//	Serial_write(0x0D); // Carriage Feed
//	Serial_write(0x18); // Cancel
//	Serial_write(0x1A); // Substitute
}
void UartHandler(UART_HandleTypeDef * uart){
	if(uart->Instance->ISR & USART_ISR_RXNE){
		_ReceivedData=(uint8_t)uart->Instance->RDR;
		__push(&RxBuffer,_ReceivedData);
	}
	if(uart->Instance->ISR & USART_ISR_TXE){
		uint8_t _TransmitData=__pop(&TxBuffer);
		if(_TransmitData!=0){
			uart->Instance->TDR=_TransmitData;
		}
		else{
			__HAL_UART_DISABLE_IT(TxBuffer.uart,UART_IT_TXE);
		}
	}
}
void Serial_peek(){
	if(RxBuffer._tail==RxBuffer._head)
		{
			return 0;
	}
	else{
		return RxBuffer._buffer[RxBuffer._tail];
	}
}
void Serial_readString(){

}
