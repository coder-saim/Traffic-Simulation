#ifndef __CLOCK_H
#define __CLOCK_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#define PLL_M 4
#define PLL_N 180
#define PLL_P 0 //0 to the sixteenth position (PLL=2)

/**
* Must use for clock configuration. System Clock speed is 180 MHz
**/
void initClock(void);
#ifdef __cplusplus
}
#endif
#endif
