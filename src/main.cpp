#include <Arduino.h>
#include "lvgl.h"

#include <TFT_eSPI.h>

#define V2_1
#define TFT_LED 33

/*Change to your screen resolution*/
static const uint16_t screenWidth	= 480;
static const uint16_t screenHeight	= 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

void setup()
{
}

void loop()
{
}
