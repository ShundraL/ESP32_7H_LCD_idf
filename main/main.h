#include <stdio.h>

#ifndef MAIN_H
#define MAIN_H
#define LED  2
#define WR   4
#define CS   15
#define DATA 5

struct dspl
{	uint8_t seg4;
	uint8_t seg3;
	uint8_t seg2;
	uint8_t seg1;
};

extern union u_tag {
	uint8_t t_array[4];
	struct dspl display;
}u;

void Display_Update_Loop(void *arg);
void Keep_Alive(void *arg);
void GPIO_Init(void);
void Beep_Enable(void);
void Beep_Disable(void);

#endif
