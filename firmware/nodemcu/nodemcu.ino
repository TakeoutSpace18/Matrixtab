#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

const int SD_card_CS_pin = 15;

const int home_wifi_led_pin = 4;

const char* home_wifi_ssid = "Keenetic-4372";
const char* home_wifi_password = "xEMiQFt3";

const char* ssid = "Matrixtab";
const char* password = "admin";

String img = "00000000000000000000000000000000000100000001110000111000001001000011000000100100000001000010010000010000000001000011100000111100000100000000100000000100000001000001000000010000000001000000010000111000001111000011100000000100000000000000000000000000000000000000000000000000000000000000000000111100000110000011110000011000001000000010010000000100001001000011100000111000000010000001100000000100001001000001000000100100000001000010010000010000001001000011100000011000000100000001100000000000000000000000000000000000";

ESP8266WebServer server(80);

bool has_SD = false;

bool handleFileRead(String path) {

  Serial.println("handleFileRead: " + path);

  if (path.endsWith("/")) {
    path += "index.htm";
  }

  String dataType = "text/plain";
  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
    else if(path.endsWith(".htm")) dataType = "text/html";
    else if(path.endsWith(".css")) dataType = "text/css";
    else if(path.endsWith(".js")) dataType = "application/javascript";
    else if(path.endsWith(".png")) dataType = "image/png";
    else if(path.endsWith(".gif")) dataType = "image/gif";
    else if(path.endsWith(".jpg")) dataType = "image/jpeg";
    else if(path.endsWith(".ico")) dataType = "image/x-icon";
    else if(path.endsWith(".xml")) dataType = "text/xml";
    else if(path.endsWith(".pdf")) dataType = "application/pdf";
    else if(path.endsWith(".zip")) dataType = "application/zip";

    File dataFile = SD.open(path.c_str());
    if(dataFile.isDirectory()){
      path += "/index.htm";
      dataType = "text/html";
      dataFile = SD.open(path.c_str());
    }

    if (!dataFile)
      return false;

    server.streamFile(dataFile, dataType);

    dataFile.close();
    return true;
}

void handleImgReceive() {
  img = server.arg("img");
  //draw_img(img);
  server.send(200, "text/plain", "OK");
}

void printExistsFiles() {
  File root = SD.open("/");
  printDirectory(root, 0);
}
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // файлов больше нет
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // у файлов есть размеры, у директорий – нет:
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void server_init() {
  
  server.on("/img", handleImgReceive);

  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  server.begin();
}

void connectWifi() {
  pinMode(home_wifi_led_pin, OUTPUT);
  Serial.println("");
  Serial.println("Trying to connect home wi-fi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(home_wifi_ssid, home_wifi_password);

  bool led = false;
  int wifi_expect_time = 5000; // Время на попытку подключится к домашнему wifi
  int start_millis = millis();
  int cur_millis;

  while (WiFi.status() != WL_CONNECTED && millis() - wifi_expect_time >= start_millis){
    Serial.print(".");

    if (led){
      digitalWrite(home_wifi_led_pin, LOW);
    }
    else{digitalWrite(home_wifi_led_pin, HIGH);}

    led = !led;
    delay(150);
  }
  Serial.println("");
  
  if (WiFi.status() == WL_CONNECTED){
    digitalWrite(home_wifi_led_pin, HIGH);
    Serial.println("Successfully!");
    Serial.println(WiFi.localIP());
  }
  else{
    digitalWrite(home_wifi_led_pin, LOW);
    Serial.println("Failure, creating access point.");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    Serial.println(WiFi.softAPIP());
  }
  delay(100);
  
  Serial.println("");
}

void SD_init() {
  if (SD.begin(SD_card_CS_pin)) {
    has_SD = true;
    Serial.println('SD card initialized');
  }
  else {
    has_SD = false;
    Serial.println('SD card error!');
  }

}


void setup()
{ 
  
  Serial.begin(115200);
  SD_init();
  printExistsFiles();
  connectWifi();
  server_init();
}

void loop()
{
  server.handleClient();
}

