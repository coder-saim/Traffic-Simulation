#ifndef __SYS_INIT_H
#define __SYS_INIT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f446xx.h"

void SysTick_Handler(void);
void sysInit(void);
uint32_t getmsTick(void);
uint8_t ms_delay(uint32_t delay);

#ifdef __cplusplus
extern "}" {
#endif
#endif
