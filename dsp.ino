 /*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
//#include <EEPROM.h>
#include "GyverEncoder.h"

#define CLK 2
#define DT 3
#define SW 4

#define NUM_LEDS 32
#define DATA_PIN 6

Encoder enc1(CLK, DT, SW);
CRGB leds[NUM_LEDS];


int value = 0;

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
*/

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

static const unsigned char PROGMEM logoBMP[] = 
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0f, 0x80, 0x0f, 0x80, 0x03, 0xc1, 0xf0, 0x7c, 0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
0x00, 0x07, 0x80, 0x0f, 0x80, 0x07, 0xc3, 0xf1, 0xff, 0x1f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
0x00, 0x07, 0x80, 0x1f, 0x00, 0x07, 0x87, 0xf3, 0xff, 0x1f, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x00, 
0x00, 0x07, 0x8f, 0x1f, 0x00, 0x07, 0x87, 0xf3, 0xff, 0x9f, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x00, 
0x00, 0x07, 0x8f, 0x3e, 0x00, 0x07, 0x87, 0x87, 0xcf, 0x84, 0x00, 0x01, 0xf0, 0x03, 0xc0, 0x00, 
0x00, 0x07, 0xdf, 0x3e, 0x1e, 0x07, 0xbf, 0xe7, 0xcf, 0x9e, 0x0f, 0x79, 0xef, 0x0f, 0xf0, 0x00, 
0x00, 0x07, 0xdf, 0x7c, 0x7f, 0x87, 0xbf, 0xc7, 0xf0, 0x1e, 0x3f, 0xf9, 0xff, 0x9f, 0xf0, 0x00, 
0x00, 0x07, 0xff, 0x7c, 0xff, 0xcf, 0x3f, 0xc7, 0xfc, 0x1e, 0x7f, 0xf9, 0xff, 0x9f, 0xe0, 0x00, 
0x00, 0x03, 0xff, 0xf9, 0xff, 0xcf, 0x3f, 0xc3, 0xfe, 0x3e, 0x7f, 0xf9, 0xff, 0xdf, 0xe0, 0x00, 
0x00, 0x03, 0xff, 0xf3, 0xe3, 0xcf, 0x0f, 0x01, 0xff, 0x3c, 0xf8, 0xf3, 0xe7, 0xc7, 0x80, 0x00, 
0x00, 0x03, 0xff, 0xf3, 0xc3, 0xcf, 0x1e, 0x00, 0x3f, 0x3c, 0xf0, 0xf3, 0xc7, 0x87, 0x80, 0x00, 
0x00, 0x03, 0xf7, 0xe3, 0xc3, 0xcf, 0x1e, 0x0f, 0x1f, 0x3c, 0xf0, 0xf3, 0xc7, 0x8f, 0x80, 0x00, 
0x00, 0x03, 0xf7, 0xe3, 0xe7, 0xdf, 0x1e, 0x0f, 0x9f, 0x3c, 0xfb, 0xf3, 0xc7, 0x8f, 0x00, 0x00, 
0x00, 0x03, 0xe7, 0xc3, 0xff, 0x9e, 0x1e, 0x0f, 0xfe, 0x7c, 0xff, 0xf3, 0xc7, 0x8f, 0x00, 0x00, 
0x00, 0x03, 0xe7, 0xc1, 0xff, 0x1e, 0x1e, 0x0f, 0xfc, 0x78, 0xff, 0xe7, 0xcf, 0x8f, 0x00, 0x00, 
0x00, 0x01, 0xc7, 0x81, 0xfe, 0x1e, 0x3e, 0x07, 0xf8, 0x78, 0x7f, 0xe7, 0x8f, 0x0f, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x0d, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x9f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x13, 0x8c, 0xda, 0x7f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x33, 0x9c, 0x45, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x3f, 0xe4, 0x0e, 0x1f, 0xc0, 0x20, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x3f, 0xe7, 0x0e, 0x7f, 0xe0, 0x70, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8e, 0x3f, 0xe7, 0x8e, 0x7f, 0xe0, 0xf8, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8e, 0x38, 0x07, 0xee, 0xf0, 0x20, 0xf8, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x3f, 0xc7, 0xfe, 0xe3, 0xf1, 0xfc, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x3f, 0xc7, 0xfe, 0xf3, 0xf3, 0xde, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x87, 0x38, 0x07, 0x3e, 0x79, 0xf3, 0xfe, 0x7f, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x3f, 0xe7, 0x1e, 0x7f, 0xe7, 0xff, 0x7f, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x3f, 0xe7, 0x06, 0x3f, 0xcf, 0x07, 0xff, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0xff, 0x83, 0xf8, 0xff, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0xff, 0xcf, 0xfc, 0xff, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x8c, 0xff, 0xcf, 0xfe, 0xff, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0xe1, 0xde, 0x0e, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0xff, 0xdc, 0x0e, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xde, 0x0e, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xff, 0x8f, 0x3e, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xe0, 0x0f, 0xfc, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0xe0, 0x07, 0xf8, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0
};

static const unsigned char PROGMEM logobsBMP[] = 
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xe0, 0x04, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc7, 0xfc, 0xc1, 0xc7, 0xfc, 0x06, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe7, 0xfc, 0xf1, 0xcf, 0xfc, 0x0e, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe7, 0x00, 0xfd, 0xcf, 0x0c, 0x1f, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc7, 0xf8, 0xff, 0xce, 0x7e, 0x1f, 0x87, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe7, 0xf8, 0xff, 0xce, 0x7e, 0x3b, 0x87, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe7, 0x00, 0xef, 0xcf, 0x7e, 0x7f, 0xc7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe7, 0xfc, 0xe3, 0xcf, 0xfe, 0x7f, 0xe7, 0xf8, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe7, 0xfc, 0xe1, 0xc7, 0xfc, 0xff, 0xf7, 0xf8, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x87, 0xfc, 0xe0, 0x41, 0xf0, 0xe0, 0x77, 0xf8, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x8f, 0xf8, 0xff, 0x9f, 0xf8, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8f, 0xf9, 0xff, 0xdf, 0xf8, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0e, 0x39, 0xe3, 0xc1, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8e, 0x3b, 0xc1, 0xc1, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xcf, 0xfb, 0x81, 0xc1, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xcf, 0xf1, 0xc3, 0xc1, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xce, 0x01, 0xff, 0xc1, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xce, 0x00, 0xff, 0x81, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0e, 0x00, 0x3e, 0x01, 0xc0, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0
};
/*
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
*/

int mode = 0;
String modes[] = {"TEMP", "HSB", "RGB", "DUET", "HORI", "STRB", "ABND", "WIRELESS"};

int T = 1000;
int I = 0;

int H = 0;
int S = 0;
int V = 0;

int Hd = 0;
int Sd = 0;
int Vd = 0;

int R = 0;
int G = 0;
int B = 0;

int del = 10;



int EEPROMmode;
unsigned long timing;
bool strdo = false;
bool strrnddo = false;

int choice = 0;


void setup(){ 
   FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  /*
  EEPROM.write(0,0);    
  EEPROMmode = EEPROM.read(0);        
  Serial.begin(9600);
  Serial.println(EEPROMmode);
  */
  
  enc1.setType(TYPE1); 
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
 // display.display();
  //delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  display.drawBitmap(1,1,logoBMP,128,32, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
display.drawBitmap(2,2,logobsBMP,128,64, WHITE);
display.display();
display.setCursor(0,0);
display.setTextSize(1);
display.setTextColor(WHITE);
display.print("v1.0      LSTR's");
display.display();
delay(3000);
display.clearDisplay();
  display.display();

 

  if(mode == 0){
    display.setCursor(0,0);
    display.print("TEMP <");
    display.setCursor(0,8);
    display.print("VAL: ");
    display.print(T);
    display.print("K");
    display.setCursor(0,16);
    display.print("BRG: ");
    display.print(I);

    display.display();
  }
  if(mode == 1){
    display.setCursor(0,0);
    display.print("HSB <");
    display.setCursor(0,8);
    display.print("H: ");
    display.print(H);
    display.setCursor(0,16);
    display.print("S: ");
    display.print(S);
    display.setCursor(0,25);
    display.print("B: ");
    display.print(V);

    display.display();
  }
   if(mode == 2){
    display.setCursor(0,0);
    display.print("RGB <");
    display.setCursor(0,8);
    display.print("R: ");
    display.print(R);
    display.setCursor(0,16);
    display.print("G: ");
    display.print(G);
    display.setCursor(0,25);
    display.print("B: ");
    display.print(B);

    display.display();
  }
  if(mode == 3){
    display.setCursor(0,0);
    display.print("DUET <");
    display.setCursor(0,8);
    display.print("1:");
    display.setCursor(20,8);
    display.print("H: ");
    display.print(H);
    display.setCursor(20,16);
    display.print("S: ");
    display.print(S);
    display.setCursor(20,25);
    display.print("B: ");
    display.print(V);
    display.setCursor(60,8);
    display.print("2:");
    display.setCursor(80,8);
     display.print("H: ");
    display.print(Hd);
    display.setCursor(80,16);
    display.print("S: ");
    display.print(Sd);
    display.setCursor(80,25);
    display.print("B: ");
    display.print(Vd);

    display.display();
  }
   if(mode == 4){
    display.setCursor(0,0);
    display.print("MOSAIC <");
    display.setCursor(0,8);
    display.print("1:");
    display.setCursor(20,8);
    display.print("H: ");
    display.print(H);
    display.setCursor(20,16);
    display.print("S: ");
    display.print(S);
    display.setCursor(20,25);
    display.print("B: ");
    display.print(V);
    display.setCursor(60,8);
    display.print("2:");
    display.setCursor(80,8);
     display.print("H: ");
    display.print(Hd);
    display.setCursor(80,16);
    display.print("S: ");
    display.print(Sd);
    display.setCursor(80,25);
    display.print("B: ");
    display.print(Vd);

    display.display();
  }
  if (mode == 5){
    display.setCursor(0,0);
    display.print("STROBE <");
    display.setCursor(0,8);
    display.print("H: ");
    display.print(H);
    display.setCursor(0,16);
    display.print("S: ");
    display.print(S);
    display.setCursor(0,25);
    display.print("B: ");
    display.print(V);
    display.setCursor(80,8);
     display.print("D: ");
     display.print(del);

    display.display();
  }
   if (mode == 6){
    display.setCursor(0,0);
    display.print("STROBE RANDOM <");
    display.setCursor(0,8);
    display.print("H: ");
    display.print(H);
    display.setCursor(0,16);
    display.print("S: ");
    display.print(S);
    display.setCursor(0,25);
    display.print("B: ");
    display.print(V);

    display.display();
  }
}


void setKelvin(int16_t temperature) {
  float tmpKelvin, tmpCalc;
  int _r, _g, _b;

  temperature = constrain(temperature, 1000, 40000);
  tmpKelvin = temperature / 100;

  // red
  if (tmpKelvin <= 66) _r = 255;
  else {
    tmpCalc = tmpKelvin - 60;
    tmpCalc = (float)pow(tmpCalc, -0.1332047592);
    tmpCalc *= (float)329.698727446;
    tmpCalc = constrain(tmpCalc, 0, 255);
    _r = tmpCalc;
  }

  // green
  if (tmpKelvin <= 66) {
    tmpCalc = tmpKelvin;
    tmpCalc = (float)99.4708025861 * log(tmpCalc) - 161.1195681661;
    tmpCalc = constrain(tmpCalc, 0, 255);
    _g = tmpCalc;
  } else {
    tmpCalc = tmpKelvin - 60;
    tmpCalc = (float)pow(tmpCalc, -0.0755148492);
    tmpCalc *= (float)288.1221695283;
    tmpCalc = constrain(tmpCalc, 0, 255);
    _g = tmpCalc;
  }

  // blue
  if (tmpKelvin >= 66) _b = 255;
  else if (tmpKelvin <= 19) _b = 0;
  else {
    tmpCalc = tmpKelvin - 10;
    tmpCalc = (float)138.5177312231 * log(tmpCalc) - 305.0447927307;
    tmpCalc = constrain(tmpCalc, 0, 255);
    _b = tmpCalc;
  }
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i].setRGB(_g, _r, _b);
  }
}


//"TEMP", "HSB", "RGB", "DUET", "HORI", "STRB", "ABND", "WIRELESS"
void loop() {
   enc1.tick();

  if (enc1.isRight()){
    if(choice == 0){
        if (mode != 6) mode++;
        else mode = 0; 
      }
      
    if(mode == 0){
      
      if(choice == 1){ if(T < 10000) T += 500; else T = 1000;}
      if(choice == 2){ if(I < 255) I += 5; else I = 0;}
    }
     if(mode == 1 || mode == 6){
      
      if(choice == 1){ if(H < 255) H += 5; else H = 0;}
      if(choice == 2){ if(S < 255) S += 5; else S = 0;}
      if(choice == 3){ if(V < 255) V += 5; else V = 0;}
    }
    if(mode == 2){
      
      if(choice == 1){ if(R < 255) R += 5; else R = 0;}
      if(choice == 2){ if(G < 255) G += 5; else G = 0;}
      if(choice == 3){ if(B < 255) B += 5; else B = 0;}
    }
    if(mode == 3 || mode == 4){
      
      if(choice == 1){ if(H < 255) H += 5; else H = 0;}
      if(choice == 2){ if(S < 255) S += 5; else S = 0;}
      if(choice == 3){ if(V < 255) V += 5; else V = 0;}
       if(choice == 4){ if(Hd < 255) Hd += 5; else Hd = 0;}
      if(choice == 5){ if(Sd < 255) Sd += 5; else Sd = 0;}
      if(choice == 6){ if(Vd < 255) Vd += 5; else Vd = 0;}
    }
     if(mode == 5){
      
      if(choice == 1){ if(H < 255) H += 5; else H = 0;}
      if(choice == 2){ if(S < 255) S += 5; else S = 0;}
      if(choice == 3){ if(V < 255) V += 5; else V = 0;}
      if(choice == 4){ if(del <1000) del += 5; else del = 5;}
    }
          // если был поворот направо, увеличиваем на 1
  }
  
  if (enc1.isLeft()){
    if(choice == 0){
        if (mode != 0) mode--;
        else mode = 6; 
      }
      
    if(mode == 0){
      
      if(choice == 1){ if(T > 1000) T -= 500; else T = 10000;}
      if(choice == 2){ if(I > 0) I -= 5; else I = 255;}
    }
     if(mode == 1 || mode == 6){
      
      if(choice == 1){ if(H > 0) H -= 5; else H = 255;}
      if(choice == 2){ if(S > 0) S -= 5; else S = 255;}
      if(choice == 3){ if(V > 0) V -= 5; else V = 255;}
    }
     if(mode == 2){
      
      if(choice == 1){ if(R > 0) R -= 5; else R = 255;}
      if(choice == 2){ if(G > 0) G -= 5; else G = 255;}
      if(choice == 3){ if(B > 0) B -= 5; else B = 255;}
    }
    if(mode == 3 || mode == 4){
      
      if(choice == 1){ if(H > 0) H -= 5; else H = 255;}
      if(choice == 2){ if(S > 0) S -= 5; else S = 255;}
      if(choice == 3){ if(V > 0) V -= 5; else V = 255;}
      if(choice == 4){ if(Hd > 0) Hd -= 5; else Hd = 255;}
      if(choice == 5){ if(Sd > 0) Sd -= 5; else Sd = 255;}
      if(choice == 6){ if(Vd > 0) Vd -= 5; else Vd = 255;}
    }
     if(mode == 5){
      
      if(choice == 1){ if(H > 0) H -= 5; else H = 255;}
      if(choice == 2){ if(S > 0) S -= 5; else S = 255;}
      if(choice == 3){ if(V > 0) V -= 5; else V = 255;}
       if(choice == 4){ if(del > 5) del -= 5; else del = 1000;}
    }
    
    
      // если был поворот налево, уменьшаем на 1
  }
  
  if (enc1.isRightH()){
    if(mode == 0){
      if(choice < 2) choice ++;
      else choice = 0;
    }
    if(mode == 1 || mode == 2 || mode == 6){
      if(choice < 3) choice ++;
      else choice = 0;
    }
    if(mode == 3 || mode == 4){
      if(choice < 6) choice ++;
      else choice = 0;
    }
    if(mode == 5){
      if(choice < 4) choice ++;
      else choice = 0;
    }
    
  }
  if (enc1.isLeftH()){
    if(mode == 0){
      if(choice > 0) choice --;
      else choice = 2;
    }
    if(mode == 1 || mode == 2 || mode == 6){
      if(choice > 0) choice --;
      else choice = 3;
    }
    if(mode == 3 || mode == 4 ){
      if(choice > 0) choice --;
      else choice = 6;
    }
    if(mode == 5){
      if(choice > 0) choice --;
      else choice = 4;
    }
    // если было удержание + поворот налево, уменьшаем на 5  
  }

  if (enc1.isTurn()) {       // если был совершён поворот (индикатор поворота в любую сторону)
    if(mode == 0){
      strrnddo = false;
      strdo = false;
     
   // leds[i] = CHSV(ahue, thissat, 255);
   for (int i = 0 ; i < 32; i++ ) {
    leds[i].setRGB( 255, 255, 255);
  }
    setKelvin(T);
    LEDS.setBrightness(I);
    LEDS.show();
  
      display.clearDisplay();
    display.setCursor(0,0);
    display.print("TEMP");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("VAL: ");
    display.print(T);
    display.print("K");
    if(choice == 1) display.print(" <");
    display.setCursor(0,16);
    display.print("BRG: ");
    display.print(I);
    if(choice == 2) display.print(" <");

    display.display();
  }
  if(mode == 1){
    strdo = false;
    strrnddo = false;
    for (int i = 0 ; i < NUM_LEDS; i++ ) {
     
    leds[i] = CHSV(H, S, V);
     LEDS.setBrightness(255);
  }
  LEDS.show();

display.clearDisplay();
   display.setCursor(0,0);
    display.print("HSB");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("H: ");
    display.print(H);
    if(choice == 1) display.print(" <");
    display.setCursor(0,16);
    display.print("S: ");
    display.print(S);
    if(choice == 2) display.print(" <");
    display.setCursor(0,25);
    display.print("B: ");
    display.print(V);
    if(choice == 3) display.print(" <");

    display.display();
  }
  if(mode == 2){
    strrnddo = false;
    strdo = false;
    LEDS.setBrightness(255);
    for (int i = 0 ; i < NUM_LEDS; i++ ) {
     
   leds[i].setRGB( G,R , B);
  }
  LEDS.show();

display.clearDisplay();
   display.setCursor(0,0);
    display.print("RGB");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("R: ");
    display.print(R);
    if(choice == 1) display.print(" <");
    display.setCursor(0,16);
    display.print("G: ");
    display.print(G);
    if(choice == 2) display.print(" <");
    display.setCursor(0,25);
    display.print("B: ");
    display.print(B);
    if(choice == 3) display.print(" <");

    display.display();
  }

  if(mode == 3){
    strrnddo = false;
    strdo = false;
     for (int i = 0 ; i < 16; i++ ) {
     
    leds[i] = CHSV(H, S, V);
     LEDS.setBrightness(255);
  }
   for (int i = 16 ; i < 32; i++ ) {
     
    leds[i] = CHSV(Hd, Sd, Vd);
     LEDS.setBrightness(255);
  }
  
  LEDS.show();
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("DUET");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("1:");
    display.setCursor(20,8);
    display.print("H: ");
    display.print(H);
    if(choice == 1) display.print("<");
    display.setCursor(20,16);
    display.print("S: ");
    display.print(S);
    if(choice == 2) display.print("<");
    display.setCursor(20,25);
    display.print("B: ");
    display.print(V);
    if(choice == 3) display.print("<");
    display.setCursor(60,8);
    display.print("2:");
    display.setCursor(80,8);
     display.print("H: ");
    display.print(Hd);
    if(choice == 4) display.print("<");
    display.setCursor(80,16);
    display.print("S: ");
    display.print(Sd);
    if(choice == 5) display.print("<");
    display.setCursor(80,25);
    display.print("B: ");
    display.print(Vd);
    if(choice == 6) display.print("<");

    display.display();
  }

  if(mode == 4){
    strrnddo = false;
    strdo = false;
  
LEDS.setBrightness(255);
   for (int i = 0 ; i < 32; i+=1 ) {
     
    leds[i] = CHSV(Hd, Sd, Vd);
     
     
  }
    
 for (int i = 0 ; i < 32; i+= 2 ) {
     
    leds[i] = CHSV(H, S, V);
     

  }
  LEDS.show();

 
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("MOSAIC");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("1:");
    display.setCursor(20,8);
    display.print("H: ");
    display.print(H);
    if(choice == 1) display.print("<");
    display.setCursor(20,16);
    display.print("S: ");
    display.print(S);
    if(choice == 2) display.print("<");
    display.setCursor(20,25);
    display.print("B: ");
    display.print(V);
    if(choice == 3) display.print("<");
    display.setCursor(60,8);
    display.print("2:");
    display.setCursor(80,8);
     display.print("H: ");
    display.print(Hd);
    if(choice == 4) display.print("<");
    display.setCursor(80,16);
    display.print("S: ");
    display.print(Sd);
    if(choice == 5) display.print("<");
    display.setCursor(80,25);
    display.print("B: ");
    display.print(Vd);
    if(choice == 6) display.print("<");

    display.display();
  }
  if(mode == 5){
    strrnddo = false;
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("STROBE");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("H: ");
    display.print(H);
    if(choice == 1) display.print("<");
    display.setCursor(0,16);
    display.print("S: ");
    display.print(S);
    if(choice == 2) display.print("<");
    display.setCursor(0,25);
    display.print("B: ");
    display.print(V);
    if(choice == 3) display.print("<");
    display.setCursor(80,8);
     display.print("D: ");
     display.print(del);
     if(choice == 4) display.print("<");

    display.display();
    strdo = true;
    //delay(200);
  }
  if(mode == 6){
    strdo = false;
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("STROBE RANDOM");
    if(choice == 0) display.print(" <");
    display.setCursor(0,8);
    display.print("H: ");
    display.print(H);
    if(choice == 1) display.print("<");
    display.setCursor(0,16);
    display.print("S: ");
    display.print(S);
    if(choice == 2) display.print("<");
    display.setCursor(0,25);
    display.print("B: ");
    display.print(V);
    if(choice == 3) display.print("<");
   

    display.display();
    strrnddo = true;
    //delay(200);
  }
  
}
if(strdo == true){
    //display.clearDisplay();
display.setCursor(80,0);
     //display.print(timing);
 
    if(millis() - timing > del){
      for (int i = 0 ; i < NUM_LEDS; i++ ) {
     
    leds[i] = CHSV(H, S, 0);
     LEDS.setBrightness(255);
  }
  LEDS.show();

    }

   
    
    if(millis() - timing > del*2){
      for (int i = 0 ; i < NUM_LEDS; i++ ) {
     
    leds[i] = CHSV(H, S, V);
     LEDS.setBrightness(255);
  }
  LEDS.show();
  timing = millis();
    }
  


  }
  if(strrnddo == true){
    //display.clearDisplay();
display.setCursor(80,0);
     //display.print(timing);
 
    if(millis() - timing > del){
      for (int i = 0 ; i < NUM_LEDS; i++ ) {
     
    leds[i] = CHSV(H, S, 0);
     LEDS.setBrightness(255);
  }
  LEDS.show();
del = random(1000);
    }

   
    
    if(millis() - timing > del*2){
      for (int i = 0 ; i < NUM_LEDS; i++ ) {
     
    leds[i] = CHSV(H, S, V);
     LEDS.setBrightness(255);
  }
  LEDS.show();
  timing = millis();
  del = random(1000);
    }
  


  }
}