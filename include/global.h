#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern float glob_temperature;
extern float glob_humidity;

extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;

// Sensor data for RTOS communication
typedef struct {
    float temperature;
    float humidity;
    uint8_t tempState;   // 0 low, 1 medium, 2 high
    uint8_t humiState;   // 0 low, 1 medium, 2 high
} SensorData;

// Task 3 RTOS objects
extern QueueHandle_t xQueueLed;
extern QueueHandle_t xQueueNeo;
extern SemaphoreHandle_t semLedTemp;
extern SemaphoreHandle_t semNeo;
#endif