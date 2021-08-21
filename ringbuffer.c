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
uint8_t __i=(unsigned int)(buff->_head + 1) % UART_BUFFER_SIZE;
while(__i == buff->_tail);
buff->_buffer[buff->_head]=a;
buff->_head=__i;
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

rxbuffer_Index_Type Serial_available(){
	 return ((rxbuffer_Index_Type)(UART_BUFFER_SIZE + RxBuffer._head - RxBuffer._tail)) % UART_BUFFER_SIZE;
}

void Serial_write(uint8_t c){
__push(&TxBuffer,c);
__HAL_UART_ENABLE_IT(TxBuffer.uart,UART_IT_TXE);
}
void Serial_print(char *datatosend){
	while(*datatosend) __push(&TxBuffer,((uint8_t)*datatosend++));
	__HAL_UART_ENABLE_IT(TxBuffer.uart,UART_IT_TXE);
}
void Serial_println(char *datatosend){
	while(*datatosend) __push(&TxBuffer,((uint8_t)*datatosend++));
	__push(&TxBuffer,0x0A);
//	__push(&TxBuffer,0x00); // NULL
//	__push(&TxBuffer,0x01); // CTRL+A Start of Header(SOH)
//	__push(&TxBuffer,0x03); // CTRL+C End of Text (ETX)
//	__push(&TxBuffer,0x04); // CTRL+D End of Transmission (EOT)
//	__push(&TxBuffer,0x05); // CTRL+E Enquiry (SOH)
//	__push(&TxBuffer,0x06); // CTRL+F Acknowledgement (ETX)
//	__push(&TxBuffer,0x07); // CTRL+G BEL
//	__push(&TxBuffer,0x0A); // New Line/LineFeed
//	__push(&TxBuffer,0x0D); // Carriage Feed
//	__push(&TxBuffer,0x18); // Cancel
//	__push(&TxBuffer,0x1A); // Substitute
	__HAL_UART_ENABLE_IT(TxBuffer.uart,UART_IT_TXE);
}
#ifdef STM32F4XX
void UartHandler(UART_HandleTypeDef * uart){
	if(uart->Instance->SR & USART_SR_RXNE){
		_ReceivedData=(uint8_t)uart->Instance->DR;
		__push(&RxBuffer,_ReceivedData);
	}
	if((uart->Instance->SR != RESET) & (USART_SR_TXE!=RESET)){
		uint8_t _TransmitData=__pop(&TxBuffer);
		if(_TransmitData!=0){
			uart->Instance->DR=_TransmitData;
		}
		else{
			__HAL_UART_DISABLE_IT(TxBuffer.uart,UART_IT_TXE);
		}
	}
}
#endif

#ifdef STM32F0XX
void UartHandler(UART_HandleTypeDef * uart){
	if(uart->Instance->ISR & USART_ISR_RXNE){
		_ReceivedData=(uint8_t)uart->Instance->RDR;
		__push(&RxBuffer,_ReceivedData);
	}
	if((uart->Instance->ISR != RESET) & (USART_ISR_TXE!=RESET)){
		uint8_t _TransmitData=__pop(&TxBuffer);
		if(_TransmitData!=0){
			uart->Instance->TDR=_TransmitData;
		}
		else{
			__HAL_UART_DISABLE_IT(TxBuffer.uart,UART_IT_TXE);
		}
	}
}
#endif
uint8_t Serial_peek(){
	if(RxBuffer._tail==RxBuffer._head)
	{
			return '\0';
	}
	else{
		return RxBuffer._buffer[RxBuffer._tail];
	}
}
char* Serial_readString(){
	rxbuffer_Index_Type datalen=((rxbuffer_Index_Type)(UART_BUFFER_SIZE + RxBuffer._head - RxBuffer._tail)) % UART_BUFFER_SIZE;
	char* buff=malloc(datalen+1);
	for(int i=0;i<datalen;i++){
		buff[i]=__pop(&RxBuffer);
	}
	buff[datalen+1]='\0';
	return buff;
}

void Serial_flush(){
	memset(RxBuffer._buffer,'\0', UART_BUFFER_SIZE);
	RxBuffer._head = 0;
	RxBuffer._tail = 0;
}

uint8_t Serial_read(){
	return __pop(&RxBuffer);
}

uint8_t Serial_find(char *searchString){
	if(strstr(&RxBuffer._buffer,searchString)!=NULL){
		return 1;
	}
	else{
		return 0;
	}
}

//int8_t* parseCSV(char){
//	if(strstr(&RxBuffer->_buffer,'\n')!=NULL){
//
//	}
//}
