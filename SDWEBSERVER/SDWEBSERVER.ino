#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266SSDP.h>
#include <SPI.h>
#include <SD.h>

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);



// Для файловой системы

static bool hasSD = false;
File uploadFile;

// Подключаем реле к ногам:
int rele1 = 16; //Pin for reley1
int rele2 = 5; //Pin for reley2


void loop() {
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
  delay(1);
  
}





