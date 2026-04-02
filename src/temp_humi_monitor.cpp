#include "temp_humi_monitor.h"
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);

void temp_humi_monitor(void *pvParameters){

    Wire.begin(11, 12);
    Serial.begin(115200);
    lcd.begin();
    lcd.backlight();
    dht20.begin();
    

    while (1){
        dht20.read();

        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();

        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Sensor Error");

            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }

        // Create data packet for RTOS communication
        SensorData data;
        data.temperature = temperature;
        data.humidity = humidity;

        // Temperature state
        if (temperature < 28) {
            data.tempState = 0;
        } 
        else if (temperature < 30) {
            data.tempState = 1;
        } 
        else {
            data.tempState = 2;
        }

        // Humidity state
        if (humidity < 60) {
            data.humiState = 0;
        } 
        else if (humidity < 85) {
            data.humiState = 1;
        } 
        else {
            data.humiState = 2;
        }

        // LCD Display
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperature, 1);
        lcd.print("C H:");
        lcd.print(humidity, 0);
        lcd.print("%");

        lcd.setCursor(0, 1);
        if (data.tempState == 2 || data.humiState == 2) {
            lcd.print("CRITICAL");
        }
        else if (data.tempState == 1 || data.humiState == 1) {
            lcd.print("WARNING");
        }
        else {
            lcd.print("NORMAL");
        }

        // Send latest data to queues
        xQueueOverwrite(xQueueLed, &data);
        xQueueOverwrite(xQueueNeo, &data);

        // Notify LED and NeoPixel tasks
        xSemaphoreGive(semLedTemp);
        xSemaphoreGive(semNeo);

        // Print the results
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}