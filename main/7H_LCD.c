
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


union u_tag u;

void app_main() 
{

	GPIO_Init();
	Display_Init();

    // Send_command(IRQ_EN);
    // Send_command(TIMER_EN);

	Clear_display();

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
	bool dot=0;

	while (1)
	{
		vTaskDelay(500 / portTICK_PERIOD_MS);
		current_time = time(NULL);
		localtime_r(&current_time,&cur_time_str);
		if(cur_time_str.tm_sec == 0)
		{
			u.display.seg4 = cur_time_str.tm_hour/10;
			u.display.seg3 = cur_time_str.tm_hour%10;
			u.display.seg2 = cur_time_str.tm_min/10;
			u.display.seg1 = cur_time_str.tm_min%10;

			Refresh_display();
		}

		if (dot)
		{
			Send_data(DDOT|MASK_B);
		}
		else
		{
			Send_data(DDOT|MASK_A);
		}
		dot = !dot;
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
