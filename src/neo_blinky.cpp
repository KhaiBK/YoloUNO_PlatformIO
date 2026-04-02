#include "neo_blinky.h"

void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    SensorData data;

    while(1) {
        if (xSemaphoreTake(semNeo, portMAX_DELAY) == pdTRUE) {
            if (xQueueReceive(xQueueNeo, &data, 0) == pdTRUE) {

                if (data.humiState == 0) {
                    // low humidity -> blue
                    strip.setPixelColor(0, strip.Color(0, 0, 255));
                }
                else if (data.humiState == 1) {
                    // medium humidity -> green
                    strip.setPixelColor(0, strip.Color(0, 255, 0));
                }
                else {
                    // high humidity -> red
                    strip.setPixelColor(0, strip.Color(255, 0, 0));
                }

                strip.show();
            }
        }
    }
}