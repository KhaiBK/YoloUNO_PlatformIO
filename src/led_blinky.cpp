#include "led_blinky.h"

void led_blinky(void *pvParameters){
    pinMode(LED_GPIO, OUTPUT);
    SensorData data;
    int currentState = 0;

    while(1) {
        if (xSemaphoreTake(semLedTemp, 0) == pdTRUE) {
            if (xQueueReceive(xQueueLed, &data, 0) == pdTRUE) {
                currentState = data.tempState;
            }
        }

        if (currentState == 0) {
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else if (currentState == 1) {
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        else {
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(200));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}