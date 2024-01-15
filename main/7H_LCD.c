
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <rom/gpio.h>
// #include "freertos/portmacro.h"
#include <portmacro.h>
#include <sdkconfig.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <time.h>
#include <sys/time.h>

#include "main.h"
#include "display.h"

#define COUNT 10
#define STACK_SIZE 1024
#define DELAY_T 450


/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blinseg2,
   or you can edit the following line and set a number here.
*/
static uint8_t seg4,seg3,seg2,seg1,flags,temp;	
time_t current_t;
struct tm tm_now; 
// static const char* TAG = "Main Module";
// static const char* TAG1 = "Function";

void app_main() 
{

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
	// ESP_LOGI("Info MSG","Init was passed");

    xTaskCreate(&Display_Update_Loop,"Display_Update_Loop",STACK_SIZE,NULL,1,NULL);
    xTaskCreate(&Keep_Alive,"Blue_LED",STACK_SIZE,NULL,1,NULL);
	
    while (1)
   {
		vTaskDelay(5000 / portTICK_PERIOD_MS);		// w\o this line WDT gets error in monitor 
   }
 }

void Display_Update_Loop(void *arg)
{
	while (1)
	{
		vTaskDelay(135 / portTICK_PERIOD_MS);
		Update_display();
	current_t = time(NULL);
		// Get_time();
		// ESP_LOGI("Diplay Task","Display Update");		    //Guru Meditation Error: Core  0 panic'ed (LoadProhibited). Exception was unhandled.
		// ESP_EARLY_LOGI("Diplay Task","Display Update");		// It works
		// ESP_DRAM_LOGI(TAG,"Display Update");					// It works
	}
}
void Keep_Alive(void *arg)
{
	while (1)
	{
		vTaskDelay(1500 / portTICK_PERIOD_MS);
		gpio_set_level(LED, 1);
		vTaskDelay(35 / portTICK_PERIOD_MS);
		gpio_set_level(LED, 0);
	}
};
void Update_display(void)
{
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

void Get_time()
{
	// current_t = time(NULL);
	// localtime_r(&current_t, &tm_now);
	// if (tm_now.tm_sec <= 30)
	// {
	// 	if (flags & (1<<DOTS))
	// 	{
	// 		flags &=~(1<<DOTS);
	// 		Send_data(DDOT|MASK_B);
	// 	}
	// 	else
	// 	{
	// 		flags |=(1<<DOTS);
	// 		Send_data(DDOT|MASK_A);
	// 	}
	// }
	// if (tm_now.tm_sec == 0)
	// {


	// }
	// ESP_EARLY_LOGI("time", "hour = %02d, min = %02d, sec= %02d", tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec); 
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
	// ESP_LOGI(TAG,"GPIO Initialized");
	// ESP_DRAM_LOGI("Test","GPIO Initialized");
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
