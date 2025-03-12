#include "display-control.h"
#include "button-config.h"
#include "time-config.h"
#include "LDE.h"

#include <EEPROM.h>

DisplayControl display;
EnhancedTimeSpan *ref;

double delta(double lastTime);

void setup() {
  Serial.begin(9600);
  display.begin();
  setupTime();
  setupButtons();
}

void loop() {
  ClickButton::checkAllClicks();
}

double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}