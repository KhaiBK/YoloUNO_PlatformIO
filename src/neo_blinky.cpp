#include "neo_blinky.h"

void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    while(1) {  

        // 🧠 Chờ tín hiệu từ temp_humi_monitor
        if (xSemaphoreTake(semNeo, portMAX_DELAY) == pdTRUE) {

            int state = neoHumiState;

            if (state == 0) {
                // 🔵 Độ ẩm thấp → xanh dương
                strip.setPixelColor(0, strip.Color(0, 0, 255));
            }
            else if (state == 1) {
                // 🟢 Trung bình → xanh lá
                strip.setPixelColor(0, strip.Color(0, 255, 0));
            }
            else {
                // 🔴 Cao → đỏ
                strip.setPixelColor(0, strip.Color(255, 0, 0));
            }

            strip.show();
        }
    }
}