
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
#include <esp_timer.h>

#include "main.h"
#include "display.h"

#define COUNT 10
#define STACK_SIZE 1024
#define STACK_SIZE2 3000
#define DELAY_T 450


/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blinseg2,
   or you can edit the following line and set a number here.
*/

struct display
{	uint8_t seg4;
	uint8_t seg3;
	uint8_t seg2;
	uint8_t seg1;
}display;


// time_t current_t;
// static const char* TAG = "Main Module";
// static const char* TAG1 = "Function";

void app_main() 
{

	GPIO_Init();
	Display_Init();

    // Send_command(IRQ_EN);
    // Send_command(TIMER_EN);

	Clear_display();
	// ESP_LOGI("Info MSG","Init was passed");

    xTaskCreate(&Display_Update_Loop,"Display_Update_Loop",STACK_SIZE2,NULL,2,NULL);
    xTaskCreate(&Keep_Alive,"Blue_LED",STACK_SIZE,NULL,1,NULL);
	
    while (1)
   {
		vTaskDelay(5000 / portTICK_PERIOD_MS);		// w\o this line WDT gets error in monitor 
   }
 }

void Display_Update_Loop(void *arg)
{
	time_t current_time;
	struct tm cur_time_str;
	struct 
	{
		int min;
		int sec;
	}tmp_time;

	tmp_time.sec=0;
	tmp_time.min=0;

	while (1)
	{
		vTaskDelay(500 / portTICK_PERIOD_MS);
		current_time = time(NULL);
		localtime_r(&current_time,&cur_time_str);
		if(cur_time_str.tm_sec == 0)
		{
			tmp_time.sec = 0;
			tmp_time.min = cur_time_str.tm_min;
			display.seg4 = tmp_time.min/10;
			display.seg3 = tmp_time.min%10;
			Write_segment_data(display.seg4,0);
			Write_segment_data(display.seg3,1);
		}
		if (cur_time_str.tm_sec >= tmp_time.sec)
		{
			tmp_time.sec = cur_time_str.tm_sec;

			display.seg2 = tmp_time.sec/10;
			display.seg1 = tmp_time.sec%10;

			Write_segment_data(display.seg2,2);
			Write_segment_data(display.seg1,3);
		}
		if (tmp_time.sec%2)
		{
			Send_data(DDOT|MASK_B);
		}
		else
		{
			Send_data(DDOT|MASK_A);
		}

		// ESP_DRAM_LOGI("Display Task","%d02,%d02",cur_time_str.tm_min,cur_time_str.tm_sec);
		// Get_time();
		// ESP_LOGI("Diplay Task","Display Update");		    //Guru Meditation Error: Core  0 panic'ed (LoadProhibited). Exception was unhandled.
		// ESP_EARLY_LOGI("Diplay Task","Display Update");		// It works
		// ESP_DRAM_LOGI(TAG,"Display Update");					// It works
	}
	vTaskDelete(NULL);
};
void Keep_Alive(void *arg)
{
	while (1)
	{
		vTaskDelay(2500 / portTICK_PERIOD_MS);
		gpio_set_level(LED, 1);
		vTaskDelay(15 / portTICK_PERIOD_MS);
		gpio_set_level(LED, 0);
	}
	vTaskDelete(NULL);
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

// void Beep_Enable(void)
// {
// 	flags |=(1<<BEEP);
// 	Send_command(TONE_4K);
// 	Send_command(TONE_ON);
// };

// void Beep_Disable(void)
// {
// 	Send_command(TONE_OFF);
// 	flags &=~(1<<BEEP);
// };
