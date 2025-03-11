#ifndef _DISPLAY_CONTROL_H_
#define _DISPLAY_CONTROL_H_

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1


class DisplayControl {
  static Adafruit_SSD1306 display;
  bool show;
  
  public:
  void begin() {
    DisplayControl::display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    DisplayControl::display.clearDisplay();
    DisplayControl::display.setTextSize(1);
	  DisplayControl::display.setTextColor(WHITE);
    DisplayControl::display.setCursor(0, 0);
    shouldDisplay(true);
  }

  void print(String str) {
    DisplayControl::display.print(str);
    updateScreen();
  }

  void printAt(String str, int col, int row) {
    DisplayControl::display.setCursor(col, row);
    print(str);
  }

  void updateScreen() {
    if (!show) return;
    DisplayControl::display.display();
  }

  void clear() {
    DisplayControl::display.clearDisplay();
  }

  void shouldDisplay(bool state) {
    if(!state) clear();
    show = state;
  }
};

Adafruit_SSD1306 DisplayControl::display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

#endif