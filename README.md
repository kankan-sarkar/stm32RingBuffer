# stm32RingBuffer
Arduino Like STM32 Ring buffer implementation 

# How to Use?
- Copy `ringbuffer.h` into `Inc\` and `ringbuffer.c` into `Src\` folder
- Add the header file in the main.c function `#include "ringbuffer.h"`
- Add the Interrupt Handler in `USARTx_IRQHandler` function which can be found in `stm32f0xx_it.c`. Inside the function add this line `UartHandler(&huartx);` Make sure to replace the `huartx` with appropiate uart handler (example huart1 , huart2 etc.)
- Add the initialization code into your main function as `Serial_begin(&huartx);` , Make sure to replace the `huartx` with appropiate uart handler 
### Thats it , Now you can use the below functions for your UART application
- `void Serial_begin(UART_HandleTypeDef *);` 
- `uint8_t Serial_available();`
- `void Serial_flush();`
- `uint8_t Serial_read();`
- `void Serial_write(uint8_t c);`
- `void Serial_print(char *);`
- `void Serial_println(char *);`
- `uint8_t Serial_find(char *);`
- `uint8_t Serial_peek();`
- `char* Serial_readString();`
- `int8_t* parseCSV(char);`
## Custom Configuration
-If you want to change the buffer size then you can change by changing the [`#define UART_BUFFER_SIZE 64`](https://github.com/trripy/stm32RingBuffer/blob/df62671de6feb61fdf4d8f6c795956167392a0fc/ringbuffer.h#L15)
-If you want to use different family of STM32 for example F4 or F0 , just comment out the inappropiate family [`#define STM32F4XX 1`](https://github.com/trripy/stm32RingBuffer/blob/df62671de6feb61fdf4d8f6c795956167392a0fc/ringbuffer.h#L18)
