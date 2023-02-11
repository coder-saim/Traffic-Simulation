#ifndef __USART_H
#define __USART_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f446xx.h>
#include <stdint.h>
void UART_SendChar(USART_TypeDef *,uint8_t);
void UART_SendString(USART_TypeDef *,const char*);
uint8_t UART_GetChar(USART_TypeDef *);

void UART_GetString(USART_TypeDef *,uint16_t,uint8_t*);
void _USART_WRITE(USART_TypeDef*,uint8_t*);
uint8_t _USART_READ(USART_TypeDef*,uint8_t *,uint16_t);
uint8_t _USART_READ_STR(USART_TypeDef*,uint8_t *,uint16_t);

// USART 2 Configuration
void UART2_Config(void);
//uint8_t * convert(uint32_t,uint8_t);


#ifdef __cplusplus
}
#endif
#endif
