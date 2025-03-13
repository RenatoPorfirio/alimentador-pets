#include "display-control.h"
#include "button-config.h"
#include "time-config.h"
#include "screens.h"

DisplayControl display;
MenuScreen menuScreen;

IScreen* currentScreen;

double delta(double lastTime);

void setup() {
  Serial.begin(9600);
  while(!Serial.availableForWrite()) delay(10);
  display.begin();
  setupButtons();
  setupTime();
  setupScreens(&display);
  currentScreen = &menuScreen;
  // Serial.println("it works at main - line 20!");

}

void loop() {
  ClickButton::checkAllClicks();
  currentScreen->render();
}

double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}