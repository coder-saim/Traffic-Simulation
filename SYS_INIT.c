#include "SYS_INIT.h"
#define SYS_CLK_SPD 180000000 

volatile static uint32_t msTicks = 0;                              /* Variable to store millisecond ticks */

// Systick ISR executes every milli-second
void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
  msTicks++;                                                /* See startup file startup_LPC17xx.s for SysTick vector */ 
}

//Need this to initilize the systick timer
void sysInit(void)
{
	SysTick_Config(SYS_CLK_SPD/1000); //1ms
}

// this function returns current millisecond count
uint32_t getmsTick(void){
	return msTicks;
}

//This function implements delay in milli-second
uint8_t ms_delay(uint32_t delay)
{
	uint32_t cms=msTicks;
	while((msTicks-cms)<delay);
	return 1;
}
