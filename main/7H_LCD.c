
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "main.h"
#include "display.h"
#include "rom/gpio.h"

#define COUNT 10
#define STACK_SIZE 1024
#define DELAY_T 450


/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blinseg2,
   or you can edit the following line and set a number here.
*/
static uint8_t seg4,seg3,seg2,seg1,flags,temp;	
// static const char* TAG_A = "Task LCD Updating";

void app_main() 
{
    static const char* TAG = "MainModule";

	GPIO_Init();
	/* Blink before running */
    gpio_set_level(LED, 1);
    vTaskDelay(DELAY_T / portTICK_PERIOD_MS);
    gpio_set_level(LED, 0);
	vTaskDelay(DELAY_T / portTICK_PERIOD_MS);
    gpio_set_level(LED, 1);
    vTaskDelay(DELAY_T / portTICK_PERIOD_MS);
    gpio_set_level(LED, 0);
	vTaskDelay(DELAY_T / portTICK_PERIOD_MS);
    
	Display_Init();

    // Send_command(IRQ_EN);
    // Send_command(TIMER_EN);

	Clear_display();
	ESP_LOGI(TAG,"Init was passed");

    xTaskCreate(&Display_Update_Loop,"Display_Update_Loop",STACK_SIZE,NULL,1,NULL);
	
    while (1)
   {
		vTaskDelay(5000 / portTICK_PERIOD_MS);		// w\o this line WDT gets error in monitor 
   }
 }

void Display_Update_Loop(void *arg)
{
	while (1)
	{
	// ESP_LOGI(TAG_A,"LCD update");
		vTaskDelay(135 / portTICK_PERIOD_MS);
		Update_display();
	}
};

void Update_display(void)
{
    // static const char* TAG_A = "LCD_Routine";
	// static uint8_t seg4,seg3,seg2,seg1,flags,temp;	
	
	seg1++;
	temp ++;
	if (temp == 3)
	{
		temp = 0;
		if (flags & (1<<DOTS))
		{
			flags &=~(1<<DOTS);
			Send_data(DDOT|MASK_B);
		}
		else
		{
			flags |=(1<<DOTS);
			Send_data(DDOT|MASK_A);
		}
	}

	if (seg1==COUNT)
	{
		if (flags & (1<<BEEP))
		{
			Beep_Disable();
		}
		seg1 = 0;
		seg2++;
		if (seg2==COUNT)
		{
			seg2 = 0;
			seg3++;
			if (seg3==COUNT)
			{
				seg3 = 0;
				seg4++;
				if (seg4==COUNT)
				{
					seg4 = 0;
					Write_segment_data(seg2,2);
					Beep_Enable();
					// Clear_display();
				}
				Write_segment_data(seg3,1);
				Write_segment_data(seg4,0);
			}
			else
			{
				Write_segment_data(seg3,1);
				Write_segment_data(seg2,2);
			}
		}
		else
		{
			Write_segment_data(seg1,3);
			Write_segment_data(seg2,2);
		}
	}
	else
	{
		Write_segment_data(seg1,3);
	}
};

void GPIO_Init(void)
{
	gpio_pad_select_gpio(WR);
    gpio_pad_select_gpio(CS);
    gpio_pad_select_gpio(DATA);
    gpio_pad_select_gpio(LED);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(WR, GPIO_MODE_OUTPUT);
    gpio_set_direction(CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(DATA, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
};

void Beep_Enable(void)
{
	flags |=(1<<BEEP);
	Send_command(TONE_4K);
	Send_command(TONE_ON);
};

void Beep_Disable(void)
{
	Send_command(TONE_OFF);
	flags &=~(1<<BEEP);
};
