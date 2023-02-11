#include "USART.h"
#include <stdint.h>
#include <stdio.h>
#include "CLOCK.h"
#include "stm32f446xx.h"



void _USART_WRITE(USART_TypeDef *usart,uint8_t *s)
{
	while (*s) UART_SendChar(usart,*s++);
}
/*****Modify according to your need *****/
uint8_t _USART_READ(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		if(i==0 && buff[i]!= 0x03F){continue;}
		n=i;
	}
	return n;
}

uint8_t _USART_READ_STR(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		n=i;
		if(buff[i]=='\0' || buff[i] == '\n' || buff[i] == ' ')
		{ 	
			buff[i]='\0';
			break;
		}
	}
	return n;
}


void UART_SendChar(USART_TypeDef *usart,uint8_t c){
	usart->DR = c;
	while(!(usart->SR & (1<<7)));
}

// Send a string to the UART PORTx
void UART_SendString(USART_TypeDef *usart,const char *s){
	while (*s) {UART_SendChar(usart,*s);s++;}
}

uint8_t UART_GetChar(USART_TypeDef *usart){
	uint8_t tmp;
	while(!(usart->SR & (1<<5)));
	tmp=(uint8_t)usart->DR;
	return tmp;
}
void UART_GetString(USART_TypeDef *uart,uint16_t size,uint8_t* buff)
{
	uint16_t i=0;
	while(size--)
	{
		uint8_t x=UART_GetChar(uart);
		buff[i]=x;
		i++;
	}
	buff[i]='\0';
		
}



/**********************************
* USART 2 Configuration
***********************************/
void UART2_Config(void){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= (1<<17); //enable UART 2
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOA->MODER |= (2<<4); //bits [5:4] -> 1:0 -->Alternate function for pin PA2
	GPIOA->MODER |= (2<<6); //bits [7:6] -> 1:0 -->Alternate function for PA3
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6); //bits [5:4] -> 1:1 -> high speed PA2; bits [7:6] -> 1:1 -> high speed PA3 
	
	GPIOA->AFR[0] |= (7<<8);//Bytes (11:10:09:08) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA2
	GPIOA->AFR[0] |= (7<<12); //Bytes (15:14:13:12) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA3
	
	//3. Enable UART on USART_CR1 rgister
	USART2->CR1 = 0x00; //clear USART
	USART2->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART2->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART2->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART2->CR1 |= (1<<2); // enable RE for receiver 
	USART2->CR1 |= (1<<3); //enable TE for transmitter
	
}

