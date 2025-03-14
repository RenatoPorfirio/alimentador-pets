#include "display-control.h"
#include "button-config.h"
#include "time-config.h"
#include "screens.h"

DisplayControl display;

double delta(double lastTime);

void setup() {
  Serial.begin(9600);
  while(!Serial.availableForWrite()) delay(10);
  display.begin();
  setupButtons();
  setupTime();
  setCurrentScreen(new DisplayTimeScreen(&display));
  // Serial.println("it works at main - line 20!");
}

void loop() {
  IScreen* currentScreen = getCurrentScreen();
  currentScreen->render();
  ClickButton::checkAllClicks();
}

double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}