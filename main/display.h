
#include <stdio.h>

#ifndef DISPLAY_H
#define DISPLAY_H

#define E1          0
#define D1          16
#define C1          32
#define DP1         48
#define E2          64
#define D2          80
#define DP2         96
#define C2          112
#define D3          128
#define E3          144
#define C3          160
#define DP3         176
#define E4          192
#define D4          208
#define C4          224
#define B4          240
#define A4          256
#define F4          272
#define G4          288
#define B3          304
#define A3          320
#define F3          336
#define G1          352
#define F1          368
#define A1          384
#define B1          400
#define G2          416
#define F2          432
#define G3          448
#define DDOT        464
#define B2          480
#define A2          496
#define MASK_A      0b0001010000001100
#define MASK_B      0b0001010000000000
#define BLANK       10
#define DOTS        1
#define BEEP        2

#define TONE_4K     0b100010000000
#define TONE_2K     0b100011000000
#define TONE_OFF    0b100000010000
#define TONE_ON     0b100000010010

#define LCD_ON      0b100000000110
#define LCD_OFF     0b100000000100
#define BIAS_COM    0b100001000000   // BIAS 1\2
// #define BIAS_COM    0b100001000010     // BIAS 1\3
#define SYS_EN      0b100000000010
#define SYS_DIS     0b100000000000
#define TIMER_EN    0b100000001100
#define IRQ_EN	    0b100100010000
#define INT_RC      0b100000110000
#define NORMAL_MODE 0b100111000110
#define WDT_DIS     0b100000001010

/* Registers definitions */

/* Function prototypes */
void Send_command(uint16_t data);
void Send_data(uint16_t data);
void Clear_display(void);
void Delay(uint16_t delay);
void Write_segment_data(uint8_t digit, uint8_t segment);
void Display_Init(void);

#endif