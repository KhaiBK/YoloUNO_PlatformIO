#include "coreiot.h"

WiFiClient espClient;
PubSubClient client(espClient);

// ================= CONNECT MQTT =================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    // ✅ FIX QUAN TRỌNG: dùng TOKEN để login
    if (client.connect(clientId.c_str(), CORE_IOT_TOKEN.c_str(), NULL)) {

      Serial.println("✅ Connected to CoreIOT Server!");

      client.subscribe("v1/devices/me/rpc/request/+");
      Serial.println("Subscribed to RPC topic");

    } else {

      Serial.print("❌ failed, rc=");
      Serial.print(client.state());
      Serial.println(" -> retry in 5s");

      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

// ================= CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial.print("Payload: ");
  Serial.println(message);

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.println("JSON parse failed");
    return;
  }

  const char* method = doc["method"];

  if (strcmp(method, "setStateLED") == 0) {
    const char* params = doc["params"];

    if (strcmp(params, "ON") == 0) {
      Serial.println("LED ON");
      // TODO: bật LED
    } else {
      Serial.println("LED OFF");
      // TODO: tắt LED
    }
  }
}

// ================= SETUP =================
void setup_coreiot() {

  // chờ WiFi sẵn sàng
  while (1) {
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY)) {
      break;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.print(".");
  }

  Serial.println("\n🌐 Internet Connected!");

  Serial.println("===== CORE IOT CONFIG =====");
  Serial.println("SERVER: " + CORE_IOT_SERVER);
  Serial.println("TOKEN: " + CORE_IOT_TOKEN);
  Serial.println("PORT: " + CORE_IOT_PORT);

  client.setServer(CORE_IOT_SERVER.c_str(), CORE_IOT_PORT.toInt());
  client.setCallback(callback);
}

// ================= TASK =================
void coreiot_task(void *pvParameters) {

  setup_coreiot();

  while (1) {

    if (!client.connected()) {
      reconnect();
    }

    client.loop();

    // gửi dữ liệu sensor
    String payload = "{\"temperature\":" + String(glob_temperature) +
                     ",\"humidity\":" + String(glob_humidity) + "}";

    client.publish("v1/devices/me/telemetry", payload.c_str());

    Serial.println("📤 Sent: " + payload);

    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}