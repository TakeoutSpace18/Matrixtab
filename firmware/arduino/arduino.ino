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




int last_brightness;
int cur_brightness;

const uint8_t left_pin = 3;
const uint8_t right_pin = 16;
const uint8_t down_pin = 15;
const uint8_t up_pin = 17;
const uint8_t back_pin = 2;

const int CS_PIN = 9;

char img[] = "00000000000000000000000000000000000100000001110000111000001001000011000000100100000001000010010000010000000001000011100000111100000100000000100000000100000001000001000000010000000001000000010000111000001111000011100000000100000000000000000000000000000000000000000000000000000000000000000000111100000110000011110000011000001000000010010000000100001001000011100000111000000010000001100000000100001001000001000000100100000001000010010000010000001001000011100000011000000100000001100000000000000000000000000000000000";

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

void matrix_init() {
  cur_brightness = map(analogRead(A0), 0, 1023, 2, 13);
  matrix.setIntensity(cur_brightness);
  for(int i=0; i<8; i++)
  {
    matrix.setRotation(i,1);
  }
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
void check_potentiometer() {
  last_brightness = cur_brightness;
  cur_brightness = map(analogRead(A0), 0, 1023, 2, 13);
  if (cur_brightness != last_brightness) {
    matrix.setIntensity(cur_brightness);
  }
}
void setup()
{ 
  Serial.begin(9600);
  matrix_init();
  draw_img(img);
  delay(1500);
  buttons_init();

}

void loop()
{
  check_buttons();
  check_potentiometer();
  gm.render(matrix);
}

