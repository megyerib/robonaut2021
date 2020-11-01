#include <ESP8266WiFi.h>
#include <espnow.h>
#include "wemos_pins.h"

uint8_t mac[2][6] = {
  {0xEC, 0xFA, 0xBC, 0x28, 0x51, 0xB2},
  {0xEC, 0xFA, 0xBC, 0x28, 0x51, 0x46}
};

int thisDev, otherDev;

void setup()
{ 
  pinMode(WEMOS_D0, INPUT);
  thisDev  = digitalRead(WEMOS_D0);
  otherDev = !thisDev;
  
  Serial.begin(1000000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  //esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(mac[otherDev], ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  static uint8_t serInBuf[250];
  static size_t serInBufLen = 0;
  static int lastSent = 0;

  size_t avail = Serial.available();
  int t = millis();
  
  Serial.readBytes(&serInBuf[serInBufLen], avail);
  serInBufLen += avail;

  if (serInBufLen > 200 ||(serInBufLen > 0 && (t - lastSent) >= 10))
  {
    esp_now_send(mac[otherDev], serInBuf, serInBufLen);
    serInBufLen = 0;
    lastSent = t;
  }
}

void OnDataSent(uint8_t* mac, uint8_t sendStatus)
{

}

void OnDataRecv(uint8_t* mac, uint8_t *incomingData, uint8_t len)
{
  Serial.write(incomingData, len);
}
