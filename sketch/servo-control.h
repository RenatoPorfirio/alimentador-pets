#ifndef _SERVO_CONTROL_H_
#define _SERVO_CONTROL_H_

#include <Servo.h>
#include <EEPROM.h>

#define CONFIG_FLAG 57
#define CONFIG_ADDR 1023
#define SPEED_ADDR 1022
#define MAX_POS_ADDR 1021
#define MIN_POS_ADDR 1020

class ServoControl {
  int pin;
  Servo servo;
  uint8_t speed;
  uint8_t maxPos;
  uint8_t minPos;

  public:
  ServoControl() { pin = -1; }

  void begin(int pin) {
    this->pin = pin;
    servo.attach(pin);
    if(EEPROM.read(CONFIG_ADDR) != CONFIG_FLAG) config();
  }

  void goTo(int targetPos) {
    int startPos = servo.read();
    int step = startPos < targetPos ? 1 : -1;
    for(int pos = startPos; pos != targetPos; pos += step) {
      servo.write(pos);
      delay(5000/speed);
    }
  }

  void goToMax() {
    goTo(maxPos);
  }

  void goToMin() {
    goTo(minPos);
  }

  bool isConfig() {
    return !(EEPROM.read(CONFIG_ADDR) ^ CONFIG_FLAG);
  }

  void calibrate() {
    goToMax();
    goToMin();
  }

  uint8_t getSpeed() { return speed; }
  uint8_t getMinPos() { return minPos; }
  uint8_t getMaxPos() { return maxPos; }

  void config(uint8_t speed=255, uint8_t maxPos=180, uint8_t minPos=0) {
    this->speed = speed ? speed : 1;
    this->maxPos = maxPos > 180 ? 180 : maxPos;
    this->minPos = minPos > maxPos ? maxPos : minPos;
    servo.write(minPos);
    EEPROM.write(CONFIG_ADDR, CONFIG_FLAG);
    EEPROM.write(SPEED_ADDR, this->speed);
    EEPROM.write(MAX_POS_ADDR, this->maxPos);
    EEPROM.write(MIN_POS_ADDR, this->minPos);
  }
};

#endif