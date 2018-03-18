#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>


#define HORIZONTAL_MATRIX 4
#define VERTCAL_MATRIX 2

const int CS_PIN = 0;

const char* home_wifi_ssid = "Keenetic-4372";
const char* home_wifi_password = "xEMiQFt3";

const char* ssid = "Matrixtab";
const char* password = "admin";


int brightness = 8; //Яркость матрицы

String img = "00000000000000000000000000000000000100000001110000111000001001000011000000100100000001000010010000010000000001000011100000111100000100000000100000000100000001000001000000010000000001000000010000111000001111000011100000000100000000000000000000000000000000000000000000000000000000000000000000111100000110000011110000011000001000000010010000000100001001000011100000111000000010000001100000000100001001000001000000100100000001000010010000010000001001000011100000011000000100000001100000000000000000000000000000000000";


int horizontal_px = HORIZONTAL_MATRIX * 8;
int vertical_px = VERTCAL_MATRIX * 8;
int num_px = horizontal_px * vertical_px;


Max72xxPanel matrix = Max72xxPanel(CS_PIN, HORIZONTAL_MATRIX, VERTCAL_MATRIX);
ESP8266WebServer server(80);

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = "text/html";
  if (SPIFFS.exists(path)) {

    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void draw_img(String data) {
    int cur_symbol = 0;
    for (int y = 0; y < vertical_px; y++)
    {
        for (int x = 0; x < horizontal_px; x++)
        {
            matrix.drawPixel(x, y, data[cur_symbol] - '0');

            cur_symbol++;
        }
    }
    matrix.write();
}

void printExistsFiles(){
  Dir dir = SPIFFS.openDir("/");
  Serial.println("");
  Serial.println("           Files:");

  while (dir.next()){
    Serial.println(dir.fileName());
  }
}

void connectWifi(){
  Serial.println("");
  Serial.println("Trying to connect home wi-fi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(home_wifi_ssid, home_wifi_password);
  for(int i = 0; i < 25; i++){
    Serial.print(".");
    delay(150);
  }
  Serial.println("");
  
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Successfully!");
    Serial.println(WiFi.localIP());
  }
  else{
    Serial.println("Failure, creating access point.");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
  }
  Serial.println("");
}


void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();
  printExistsFiles();
  connectWifi();

  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  server.begin();

  matrix.setIntensity(brightness);
  for(int i=0; i<8; i++)
  {
    matrix.setRotation(i,1);
  }



draw_img(img);
}

void loop()
{
  server.handleClient();
}

