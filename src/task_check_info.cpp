#include "task_check_info.h"

void Load_info_File()
{
  File file = LittleFS.open("/info.dat", "r");
  if (!file)
  {
    return;
  }

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, file);

  if (!error)
  {
    WIFI_SSID = doc["WIFI_SSID"].as<String>();
    WIFI_PASS = doc["WIFI_PASS"].as<String>();
    CORE_IOT_TOKEN = doc["CORE_IOT_TOKEN"].as<String>();
    CORE_IOT_SERVER = doc["CORE_IOT_SERVER"].as<String>();
    CORE_IOT_PORT = doc["CORE_IOT_PORT"].as<String>();
  }
  else
  {
    Serial.println("JSON parse failed");
  }

  file.close();
}

void Delete_info_File()
{
  if (LittleFS.exists("/info.dat"))
  {
    LittleFS.remove("/info.dat");
  }
  ESP.restart();
}

void Save_info_File(String wifi_ssid, String wifi_pass,
                    String CORE_IOT_TOKEN,
                    String CORE_IOT_SERVER,
                    String CORE_IOT_PORT)
{
  DynamicJsonDocument doc(4096);

  doc["WIFI_SSID"] = wifi_ssid;
  doc["WIFI_PASS"] = wifi_pass;
  doc["CORE_IOT_TOKEN"] = CORE_IOT_TOKEN;
  doc["CORE_IOT_SERVER"] = CORE_IOT_SERVER;
  doc["CORE_IOT_PORT"] = CORE_IOT_PORT;

  File file = LittleFS.open("/info.dat", "w");

  if (file)
  {
    serializeJson(doc, file);
    file.close();
  }
  else
  {
    Serial.println("Save file failed");
  }

  ESP.restart();
}

bool check_info_File(bool check)
{
  if (!check)
  {
    if (!LittleFS.begin(true))
    {
      Serial.println("❌ LittleFS init failed");
      return false;
    }

    // ⚠️ TẮT LOAD FILE để tránh ghi đè config
    // Load_info_File();

    // 🔥 HARD-CODE (TEST NHANH)
    WIFI_SSID = "phuong";
    WIFI_PASS = "123456789";

    CORE_IOT_TOKEN = "Eul4ZMWd3kJU53aFvcfG";
    CORE_IOT_SERVER = "app.coreiot.io";
    CORE_IOT_PORT = "1883";

    Serial.println("===== CONFIG =====");
    Serial.println("SSID: " + WIFI_SSID);
    Serial.println("PASS: " + WIFI_PASS);
    Serial.println("TOKEN: " + CORE_IOT_TOKEN);
    Serial.println("SERVER: " + CORE_IOT_SERVER);
    Serial.println("PORT: " + CORE_IOT_PORT);
  }

  // nếu không có SSID thì bật AP
  if (WIFI_SSID.isEmpty())
  {
    if (!check)
    {
      startAP();
    }
    return false;
  }

  return true;
}