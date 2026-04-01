#include "led_blinky.h"

void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);
  
  while(1) {                        
    if (xSemaphoreTake(semLedTemp, portMAX_DELAY) == pdTRUE) {
    int state = ledTempState;

    if (state == 0) {
        // Nhiệt độ thấp: blink chậm
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (state == 1) {
        // Nhiệt độ trung bình: blink vừa
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    else {
        // Nhiệt độ cao: blink nhanh
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(pdMS_TO_TICKS(200));
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
  }
}