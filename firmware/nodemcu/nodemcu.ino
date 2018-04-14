#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>



const int home_wifi_led_pin = 4;

const char* home_wifi_ssid = "Keenetic-4372";
const char* home_wifi_password = "xEMiQFt3";

const char* ssid = "Matrixtab";
const char* password = "admin";

String img = "00000000000000000000000000000000000100000001110000111000001001000011000000100100000001000010010000010000000001000011100000111100000100000000100000000100000001000001000000010000000001000000010000111000001111000011100000000100000000000000000000000000000000000000000000000000000000000000000000111100000110000011110000011000001000000010010000000100001001000011100000111000000010000001100000000100001001000001000000100100000001000010010000010000001001000011100000011000000100000001100000000000000000000000000000000000";

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

void handleImgReceive() {
  img = server.arg("img");
  draw_img(img);
  server.send(200, "text/plain", "OK");
}

void printExistsFiles() {
  Dir dir = SPIFFS.openDir("/");
  Serial.println("");
  Serial.println("           Files:");

  while (dir.next()){
    Serial.println(dir.fileName());
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
  }
  delay(100);
  
  Serial.println("");
}




void setup()
{ 
  

  Serial.begin(115200);
  SPIFFS.begin();
  printExistsFiles();
  connectWifi();
  server_init();

}

void loop()
{
  server.handleClient();
}

