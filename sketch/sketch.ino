#include "display-control.h"
#include "button-config.h"
#include "time-manager.h"
#include "LDE.h"

#include <EEPROM.h>

DisplayControl display;
TimeManager timeManager;

double delta(double lastTime);

void setup() {
  Serial.begin(9600);
  display.begin();
  timeManager.begin();
  setupButtons();
}

void loop() {
  static double lastTime = 0;
  double dt = delta(lastTime);

  buttons.forEach([](ClickButton* button) {
    button->checkClick();
  });

  if (dt > 1000) {
    lastTime += dt;
    DateTime now = rtc.now();
    display.clear();
    display.printAt(timeManager.dateTimeToString(now), 0, 0);
  }
}

double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}