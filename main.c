#include "stm32f446xx.h"
#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "time.h"

enum
{
	A,
	B,
	C
};

typedef struct
{
	int green;
	int yellow;
	int red;
} traffic;

traffic normal = {18000, 6000, 24000};
traffic delayed = {24000, 6000, 30000};
traffic mode;

typedef struct
{
	int Port;
	uint16_t Pin;
} PortPin;

typedef struct
{
	PortPin Green;
	PortPin Yellow;
	PortPin Red;
	PortPin White[2][3];

} lane;

lane Vertical, Horizontal;

GPIO_TypeDef *Find_Port(int Port_no)
{
	if (Port_no == A)
		return GPIOA;
	else if (Port_no == B)
		return GPIOB;
	else if (Port_no == C)
		return GPIOC;
}

int trafficGenerator(void)
{
	return rand() % 3 + 1;
}

int max(int a, int b){
	if(a>b) return a;
	else return b;
}

void handle_lights(lane a, lane b, int x, int y)
{
		int i, j, k;
	
			for (j = 0; j < x; j++)
					GPIO_WritePin(Find_Port(b.White[0][j].Port), b.White[0][j].Pin, GPIO_PIN_RESET);
			
			for (j = 0; j < y; j++)
				GPIO_WritePin(Find_Port(b.White[1][j].Port), b.White[1][j].Pin, GPIO_PIN_RESET);
			
			for (j = 0; j < 6000 / 600; j++)
			{
				for (k = 0; k < 3; k++)
				{
					GPIO_WritePin(Find_Port(a.White[0][k].Port), a.White[0][k].Pin, GPIO_PIN_RESET);
					GPIO_WritePin(Find_Port(a.White[1][k].Port), a.White[1][k].Pin, GPIO_PIN_RESET);
					ms_delay(200);
					GPIO_WritePin(Find_Port(a.White[0][k].Port), a.White[0][k].Pin, GPIO_PIN_SET);
					GPIO_WritePin(Find_Port(a.White[1][k].Port), a.White[1][k].Pin, GPIO_PIN_SET);
				}
			}
			for (j = 0; j < x; j++)
					GPIO_WritePin(Find_Port(b.White[0][j].Port), b.White[0][j].Pin, GPIO_PIN_SET);
			
			for (j = 0; j < y; j++)
				GPIO_WritePin(Find_Port(b.White[1][j].Port), b.White[1][j].Pin, GPIO_PIN_SET);
}
void handle_traffic(lane a, lane b, traffic *state)
{
		int i, j, k, x, y;
		int mx1=-1,mx2=-1;

		GPIO_WritePin(Find_Port(a.Green.Port), a.Green.Pin, GPIO_PIN_RESET); // start g1
		GPIO_WritePin(Find_Port(b.Red.Port), b.Red.Pin, GPIO_PIN_RESET); // start r2
	
		for (i = 0; i < state->green / 6000; i++)
		{
			x = trafficGenerator(); mx1 = max(mx1,x); x = mx1;
			y = trafficGenerator(); mx2 = max(mx2,y); y = mx2;
			
			handle_lights(a,b,x,y);
		}
		GPIO_WritePin(Find_Port(a.Green.Port), a.Green.Pin, GPIO_PIN_SET);	 // stop g1
		GPIO_WritePin(Find_Port(a.Yellow.Port), a.Yellow.Pin, GPIO_PIN_RESET); // start y1
	
		
	 
		for (i = 0; i < state->yellow / 6000; i++)
		{
			x = trafficGenerator(); mx1 = max(mx1,x); x = mx1;
			y = trafficGenerator(); mx2 = max(mx2,y); y = mx2;
			handle_lights(a,b,x,y);			
		}
			
		GPIO_WritePin(Find_Port(a.Yellow.Port), a.Yellow.Pin, GPIO_PIN_SET);	 // stop y1
		GPIO_WritePin(Find_Port(b.Red.Port), b.Red.Pin, GPIO_PIN_SET); // stop r2
	
		if (x<3 && y<3)
			*state=delayed;
		else
			*state=normal;
}
void init_pins()
{
	Vertical.Green.Port = A;
	Vertical.Green.Pin = 7;
	Vertical.Yellow.Port = A;
	Vertical.Yellow.Pin = 6;
	Vertical.Red.Port = A;
	Vertical.Red.Pin = 5;
	Vertical.White[0][0].Port = B;
	Vertical.White[0][0].Pin = 1;
	Vertical.White[0][1].Port = B;
	Vertical.White[0][1].Pin = 2;
	Vertical.White[0][2].Port = B;
	Vertical.White[0][2].Pin = 5;
	Vertical.White[1][0].Port = B;
	Vertical.White[1][0].Pin = 8;
	Vertical.White[1][1].Port = B;
	Vertical.White[1][1].Pin = 7;
	Vertical.White[1][2].Port = B;
	Vertical.White[1][2].Pin = 6;

	Horizontal.Green.Port = A;
	Horizontal.Green.Pin = 4;
	Horizontal.Yellow.Port = A;
	Horizontal.Yellow.Pin = 1;
	Horizontal.Red.Port = A;
	Horizontal.Red.Pin = 0;
	Horizontal.White[0][0].Port = A;
	Horizontal.White[0][0].Pin = 12;
	Horizontal.White[0][1].Port = A;
	Horizontal.White[0][1].Pin = 11;
	Horizontal.White[0][2].Port = B;
	Horizontal.White[0][2].Pin = 0;
	Horizontal.White[1][0].Port = A;
	Horizontal.White[1][0].Pin = 8;
	Horizontal.White[1][1].Port = A;
	Horizontal.White[1][1].Pin = 9;
	Horizontal.White[1][2].Port = A;
	Horizontal.White[1][2].Pin = 10;	
}
int main(void)
{
	init_pins();
	
	traffic mode = normal;

	initClock();
	sysInit();

	GPIO_InitTypeDef z;
	
	z.Alternate=0;
	z.Pull=0;
	z.Speed=0;
	z.Pin=16;
	z.Mode=0;

	GPIO_Init(GPIOA, &z);
	GPIO_Init(GPIOB, &z);

	while (1)
	{	
		handle_traffic(Vertical, Horizontal, &mode);
		handle_traffic(Horizontal, Vertical, &mode);

/*
		GPIO_WritePin(GPIOA, 5, GPIO_PIN_RESET);
		ms_delay(1000);
		GPIO_WritePin(GPIOA, 5, GPIO_PIN_SET);
		ms_delay(1000);		
*/		
	}
}