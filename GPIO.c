#include "GPIO.h"
#include "CLOCK.h"

void GPIO_WritePin(GPIO_TypeDef *GPIOx,uint16_t GPIO_pin,GPIO_PinState PinState)
{
		if (PinState==0)
			GPIOx->BSRR |= (1<<GPIO_pin);
		else
			GPIOx->BSRR |= ((1<<GPIO_pin)<<16);
}
void GPIO_Init(GPIO_TypeDef* GPIOx,GPIO_InitTypeDef *GPIO_Init)
{	

		RCC->AHB1ENR |= (1<<0);
		RCC->AHB1ENR |= (1<<1);
	
		int i=0;
	
		for (i=0; i<16; i++)
		{
			GPIOx->MODER |= (1<<2*i);
		}
		
		GPIOx->OTYPER = GPIO_Init->Alternate;
		GPIOx->OSPEEDR = GPIO_Init->Speed;
}
