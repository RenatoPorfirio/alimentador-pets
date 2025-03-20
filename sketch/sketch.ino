#include "servo-control.h"
#include "display-control.h"
#include "button-config.h"
#include "time-config.h"
#include "screens/screens.h"

DisplayControl display;
ServoControl servo;
int secondsToWait = 5;

double delta(double lastTime);

void setup() {
  Serial.begin(9600);
  while (!Serial.availableForWrite()) delay(10);
  display.begin();
  servo.config(120, 92, 0);
  servo.begin(8);
  servo.calibrate();
  
  setupButtons();
  setupTime();
  setCurrentScreen(new DisplayTimeScreen(&display));
}

void loop() {
  static double lastTime = 0;
  double dt = delta(lastTime);
  
  IScreen* currentScreen = getCurrentScreen();
  currentScreen->render();
  ClickButton::checkAllClicks();

  if (dt > 1000) {
    timeRemaining.decrease(1);
    if (timeRemaining.totalseconds() <= 0 && timeQueue.size() > 0) {
      EnhancedDateTime* dt = timeQueue.pop();
      servo.goToMax();
      delay(secondsToWait * 1000);
      servo.goToMin();
      DateTime now = rtc.now();
      timeQueue.push(new EnhancedDateTime(now.year(), now.month(), now.day() + 1, dt->hour(), dt->minute(), dt->second()));
      delete dt;
      timeRemainingUpdate();
    }
    lastTime += dt;
  }
}

double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}
