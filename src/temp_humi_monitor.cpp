#include "temp_humi_monitor.h"
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);


void temp_humi_monitor(void *pvParameters){

    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();

    while (1){
        /* code */
        
        dht20.read();
        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();

        

        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
        }

        //Update global variables for temperature and humidity
        glob_temperature = temperature;
        glob_humidity = humidity;

        // Task 1: phân loại nhiệt độ cho LED
        if (temperature < 28) {
            ledTempState = 0;
        } else if (temperature < 35) {
            ledTempState = 1;
        } else {
            ledTempState = 2;
        }

        // Báo cho LED task có dữ liệu mới
        xSemaphoreGive(semLedTemp);

        // Task 2 phân loại độ ẩm theo màu

        if (humidity < 50) {
          neoHumiState = 0;
        } else if (humidity < 70) {
          neoHumiState = 1;
        } else {
         neoHumiState = 2;
        }

        xSemaphoreGive(semNeo);

        // Print the results
        
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
        vTaskDelay(5000);
    }
    
}