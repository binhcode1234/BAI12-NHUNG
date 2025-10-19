#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>


typedef struct {
    uint32_t freq;   // T?n s? (Hz)
    uint32_t duty;   // Ð? r?ng xung (%)
} BlinkParam_t;


#define LED_PORT GPIOC
#define LED_PIN  GPIO_Pin_14
#define LED_CLK  RCC_APB2Periph_GPIOC

void GPIO_Config(void);
void TaskBlink(void *pvParameters);
void TaskGen(void *pvParameters);

QueueHandle_t xBlinkQueue;

int main(void)
{
    SystemInit();
    GPIO_Config();

    xBlinkQueue = xQueueCreate(5, sizeof(BlinkParam_t));

    if (xBlinkQueue != NULL)
    {
        // Tao task chap LED (uu tiên cao hon)
        xTaskCreate(TaskBlink, "Blink", 256, NULL, 2, NULL);
        // Tao task phát du lieu (uu tiên thap hon)
        xTaskCreate(TaskGen, "Gen", 256, NULL, 1, NULL);

        // Bat dau chay FreeRTOS
        vTaskStartScheduler();
    }

    while (1);
}

// Cau hình chân LED (PC13)

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    GPIO_SetBits(LED_PORT, LED_PIN); // LED tat (ngu?c logic)
}

//-----------------------------------------------
// Task 1: Nh?n d? li?u và Blink LED
//-----------------------------------------------
void TaskBlink(void *pvParameters)
{
    BlinkParam_t param = {1, 50}; // M?c d?nh 1Hz - duty 50%
    TickType_t onTime = 500, offTime = 500;

    for (;;)
    {
        
        if (xQueueReceive(xBlinkQueue, &param, pdMS_TO_TICKS(10)) == pdPASS)
        {
            // Tránh chia 0
            if (param.freq == 0) param.freq = 1;
            if (param.duty > 100) param.duty = 100;

            
            uint32_t period = 1000 / param.freq;
            onTime = (period * param.duty) / 100;
            offTime = period - onTime;
        }

        // BAT LED
        GPIO_ResetBits(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(onTime));

        // TAT LED
        GPIO_SetBits(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(offTime));
    }
}
void TaskGen(void *pvParameters)
{
    BlinkParam_t param;
    srand(1234);

    for (;;)
    {
      
        param.freq = (rand() % 5) + 1;       // 1–5 Hz
        param.duty = (rand() % 80) + 10;     // 10–90%

        xQueueSend(xBlinkQueue, &param, pdMS_TO_TICKS(10));

        vTaskDelay(pdMS_TO_TICKS(2000)); // dung lai 2 s roi gui tiep dê thay doi kieu chop
    }
}
