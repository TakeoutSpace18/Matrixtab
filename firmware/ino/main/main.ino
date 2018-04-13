#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <AceButton.h>

using namespace ace_button;

#define HORIZONTAL_MATRIX 4
#define VERTCAL_MATRIX 2







class MoovingPoint
{
    int x;
    int y;
public:
    MoovingPoint()
    {
        x = 5;
        y = 5;
    }

    void handleEvent(uint8_t btn_num, uint8_t type)
    {
        if(type == 1)
        {
            switch(btn_num){

                case 1:
                    y--;
                    break;
                case 2:
                    x++;
                    break;
                case 3:
                    y++;
                    break;
                case 4:
                    x--;
                    break;
                case 5:
                    x--;
            }
        }
        
    }

    void render(Max72xxPanel mtx)
    {   
        mtx.fillScreen(LOW);
        mtx.drawPixel(x,y, HIGH);
        mtx.write();
    }
};







const uint8_t left_pin = 2;
const uint8_t right_pin = 16;
const uint8_t down_pin = 5;
const uint8_t up_pin = 12;
const uint8_t back_pin = 15;

const int CS_PIN = 0;
const int home_wifi_led_pin = 4;

const char* home_wifi_ssid = "Keenetic-4372";
const char* home_wifi_password = "xEMiQFt3";

const char* ssid = "Matrixtab";
const char* password = "admin";


int brightness = 8; //Яркость матрицы

String img = "00000000000000000000000000000000000100000001110000111000001001000011000000100100000001000010010000010000000001000011100000111100000100000000100000000100000001000001000000010000000001000000010000111000001111000011100000000100000000000000000000000000000000000000000000000000000000000000000000111100000110000011110000011000001000000010010000000100001001000011100000111000000010000001100000000100001001000001000000100100000001000010010000010000001001000011100000011000000100000001100000000000000000000000000000000000";


int horizontal_px = HORIZONTAL_MATRIX * 8;
int vertical_px = VERTCAL_MATRIX * 8;
int num_px = horizontal_px * vertical_px;


ButtonConfig btn_config;
AceButton btn_up(up_pin);
AceButton btn_down(down_pin);
AceButton btn_right(right_pin);
AceButton btn_left(left_pin);
AceButton btn_back(back_pin);

MoovingPoint gm;

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

void handleImgReceive() {
  img = server.arg("img");
  draw_img(img);
  server.send(200, "text/plain", "OK");
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

void printExistsFiles() {
  Dir dir = SPIFFS.openDir("/");
  Serial.println("");
  Serial.println("           Files:");

  while (dir.next()){
    Serial.println(dir.fileName());
  }
}
 
void matrix_init() {
  matrix.setIntensity(brightness);
  for(int i=0; i<8; i++)
  {
    matrix.setRotation(i,1);
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

void buttons_init() {
  btn_config.setFeature(ButtonConfig::kFeatureClick);
  btn_config.setEventHandler(handleButton);

  pinMode(up_pin, INPUT_PULLUP);
  pinMode(down_pin, INPUT_PULLUP);
  pinMode(right_pin, INPUT_PULLUP);
  pinMode(left_pin, INPUT_PULLUP);
  pinMode(back_pin, INPUT_PULLUP);

  btn_back.setButtonConfig(&btn_config);
  btn_up.setButtonConfig(&btn_config);
  btn_left.setButtonConfig(&btn_config);
  btn_right.setButtonConfig(&btn_config);
  btn_down.setButtonConfig(&btn_config);
}

void check_buttons() {
  btn_back.check();
  btn_up.check();
  btn_left.check();
  btn_right.check();
  btn_down.check();
}
void handleButton(AceButton* button, uint8_t eventType, uint8_t buttonState){
  uint8_t pin = button->getPin();
  uint8_t btn_num;
  uint8_t type;
  switch(pin){    
    case left_pin:
      btn_num = 4;
      break;
    case up_pin:
      btn_num = 1;
      break;
    case down_pin:
      btn_num = 3;
      break;
    case right_pin:
      btn_num = 2;
      break;

    case back_pin:
      btn_num = 5;
  }

  switch(eventType){
    case AceButton::kEventPressed:
      type = 1;
      break;
    case AceButton::kEventReleased:
      type = 2;
      break;
    case AceButton::kEventClicked:
      type = 3;
  }
  gm.handleEvent(btn_num, type);
}

void setup()
{ 
  matrix_init();
  draw_img(img);
  Serial.begin(115200);
  SPIFFS.begin();
  printExistsFiles();
  connectWifi();
  buttons_init();
  server_init();

}

void loop()
{
  check_buttons();
  gm.render(matrix);
  server.handleClient();
}

