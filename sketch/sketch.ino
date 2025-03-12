#include "app-defs.h"

MenuScreen menuScreen;

double delta(double lastTime);

void setup() {
  Serial.begin(9600);
  display.begin();
  setupTime();
  setupButtons();
  Serial.println("it works at main - line 11!");
  menuScreen.begin();
  Serial.println("it works at main - line 14!");
  // Context::setCurrentScreen(&menuScreen);

}

void loop() {
  ClickButton::checkAllClicks();
  // IScreen* currentScreen = Context::getCurrentScreen();
  // if(currentScreen) currentScreen->render();
  menuScreen.render();
}

double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}