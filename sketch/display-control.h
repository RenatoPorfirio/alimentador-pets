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
    DisplayControl::display.setCursor(1, 1);
    shouldDisplay(true);
  }

  void setCursor(int col, int row) {
    DisplayControl::display.setCursor(col, row);
  }

  void print(String str) {
    DisplayControl::display.print(str);
    updateScreen();
  }

  void printAt(String str, int col, int row) {
    setCursor(col, row);
    print(str);
  }

  void printBigger(String str, int size=2) {
    DisplayControl::display.setTextSize(size);
    print(str);
    DisplayControl::display.setTextSize(1);
  }

  void printBiggerAt(String str, int col, int row, int size=2) {
    DisplayControl::display.setTextSize(size);
    printAt(str, col, row);
    DisplayControl::display.setTextSize(1);
  }

  void updateScreen() {
    if (!show) return;
    DisplayControl::display.display();
  }

  void clear() {
    DisplayControl::display.clearDisplay();
  }

  void eraseFrame(int col, int row, int width, int height) {
    DisplayControl::display.fillRect(col, row, width, height, BLACK);
  }

  void shouldDisplay(bool state) {
    if(state) {
      DisplayControl::display.ssd1306_command(SSD1306_DISPLAYON);
      DisplayControl::display.ssd1306_command(SSD1306_SEGREMAP);
      DisplayControl::display.ssd1306_command(SSD1306_COMSCANDEC);
    } else {
      DisplayControl::display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
    show = state;
  }
};

Adafruit_SSD1306 DisplayControl::display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

#endif