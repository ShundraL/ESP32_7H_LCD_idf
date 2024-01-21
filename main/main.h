#include <stdio.h>

#ifndef MAIN_H
#define MAIN_H
#define LED  2
#define WR   4
#define CS   15
#define DATA 5

static struct display
{	uint8_t seg4;
	uint8_t seg3;
	uint8_t seg2;
	uint8_t seg1;
}display;

void Display_Update_Loop(void *arg);
void Keep_Alive(void *arg);
void GPIO_Init(void);
void Beep_Enable(void);
void Beep_Disable(void);

#endif
