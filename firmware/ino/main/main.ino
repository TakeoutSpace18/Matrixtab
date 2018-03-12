#include <dummy.h>

#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#include <file.h> 



#define HORIZONTAL_MATRIX 4
#define VERTCAL_MATRIX 2

int CS_PIN = 0;


int brightness = 8; //Яркость матрицы
String img = "00000000000000000000000000000000000100000001110000111000001001000011000000100100000001000010010000010000000001000011100000111100000100000000100000000100000001000001000000010000000001000000010000111000001111000011100000000100000000000000000000000000000000000000000000000000000000000000000000111100000110000011110000011000001000000010010000000100001001000011100000111000000010000001100000000100001001000001000000100100000001000010010000010000001001000011100000011000000100000001100000000000000000000000000000000000";
int cur_symbol = 0;

int horizontal_px = HORIZONTAL_MATRIX * 8;
int vertical_px = VERTCAL_MATRIX * 8;
int num_px = horizontal_px * vertical_px;


Max72xxPanel matrix = Max72xxPanel(CS_PIN, HORIZONTAL_MATRIX, VERTCAL_MATRIX);


void draw_img(String data)
{
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


void setup()
{
matrix.setIntensity(brightness);
for(int i=0; i<8; i++)
{
  matrix.setRotation(i,1);
}

draw_img(img);
}

void loop()
{

}

