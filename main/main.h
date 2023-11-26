#include <stdio.h>

#ifndef MAIN_H
#define MAIN_H
#define LED  2
#define WR   4
#define CS   15
#define DATA 5

void Display_Update_Loop(void *arg);
void Update_display(void);
void GPIO_Init(void);
void Beep_Enable(void);
void Beep_Disable(void);

#endif
