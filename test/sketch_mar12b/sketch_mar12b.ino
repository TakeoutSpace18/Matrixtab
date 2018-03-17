#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

const char* ssid = "ESP"; // запускаем WiFi точку ESP

ESP8266WebServer server(80); // запускаем сервер на порту 80
Max72xxPanel matrix = Max72xxPanel(D8, 4, 1);

unsigned long ticker_next;
String tape = "RobotClass";
int spacer = 1;
int width = 5 + spacer;

// HTML страница index.html
const char page[] = 
"<html>"
"<title>Ticker control</title>"
"<body>"
"<form action=\"\" method=\"post\">"
"<input type=\"text\" name=\"text\"/>"
"<input type=\"submit\" value=\"Set text\">"
"</form>"
"</body>"
"</html>";

// функция вызывается, когда клиент жмет кнопку
void handleSubmit(){
    tape = server.arg("text");
    server.send(200, "text/html", page);
}

void handleRoot() {
    if (server.hasArg("text")) {
        handleSubmit();
    }
    else {
        server.send(200, "text/html", page);
    }
}

void setup(void){
    delay(1000);
    WiFi.softAP(ssid);

    server.on("/", handleRoot);
    server.begin();

   matrix.setIntensity(7);
}

void handleTicker(){
    for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
        matrix.fillScreen(LOW);

        int letter = i / width;
        int x = (matrix.width() - 1) - i % width;
        int y = (matrix.height() - 8) / 2; // центровка по вертикали

        while ( x + width - spacer >= 0 && letter >= 0 ) {
            if ( letter < tape.length() ) {
                matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
                server.handleClient();
            }

            letter--;
            x -= width;
        }

        matrix.write();
         delay(50);
    }
}

void loop(void){
    server.handleClient();
    handleTicker();
}
