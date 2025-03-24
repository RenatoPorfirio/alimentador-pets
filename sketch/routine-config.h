#ifndef _ROUTINE_CONFIG_H_
#define _ROUTINE_CONFIG_H_

int secondsToWait = 5;
ServoControl servo;

void setupRoutine(bool force_config=false) {
  if (!servo.isConfig() || force_config) {
    servo.config(120, 92, 0);
  }
  servo.begin(8);

  servo.calibrate();
}

void routineTimeConfig() {
  
}

#endif