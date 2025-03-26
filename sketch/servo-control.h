#ifndef _SERVO_CONTROL_H_
#define _SERVO_CONTROL_H_

#include <Servo.h>
#include <EEPROM.h>

#define CONFIG_FLAG 57          // Valor para verificar se o servo foi configurado
#define CONFIG_ADDR 1023        // Endereço de memória para verificar se o servo foi configurado
#define SPEED_ADDR 1022         // Endereço de memória para armazenar a velocidade
#define MAX_POS_ADDR 1021       // Endereço de memória para armazenar a posição máxima
#define MIN_POS_ADDR 1020       // Endereço de memória para armazenar a posição mínima

/*
  * ServoControl
  * Classe para controle de servo motor
  * Propriedades:
  * - pin: pino de controle do servo
  * - servo: objeto Servo
  * - speed: velocidade do servo
  * - maxPos: posição máxima do servo
  * - minPos: posição mínima do servo
  * - started: indica se o servo foi inicializado
  * Métodos:
  * - begin(int pin): inicializa o servo
  * - goTo(int targetPos): move o servo para uma posição
  * - goToMax(): move o servo para a posição máxima
  * - goToMin(): move o servo para a posição mínima
  * - isConfig(): verifica se o servo foi configurado
  * - calibrate(): calibra o servo
  * - getSpeed(): retorna a velocidade do servo
  * - getMinPos(): retorna a posição mínima do servo
  * - getMaxPos(): retorna a posição máxima do servo
  * - config(uint8_t speed, uint8_t maxPos, uint8_t minPos): configura o servo
  */
class ServoControl {
  int pin;
  Servo servo;
  uint8_t speed;
  uint8_t maxPos;
  uint8_t minPos;
  bool started;

  public:
  ServoControl() { pin = -1; }

  void begin(int pin) {
    started = false;
    this->pin = pin;
    servo.attach(pin);
    if(!isConfig()) config();
    else {
      this->speed = EEPROM.read(SPEED_ADDR);
      this->minPos = EEPROM.read(MIN_POS_ADDR);
      this->maxPos = EEPROM.read(MAX_POS_ADDR);
    }
    goToMin();
    started = true;
  }

  void goTo(int targetPos) {
    if (!started) {
      servo.write(minPos);
      return;
    }
    int startPos = servo.read();
    int step = startPos < targetPos ? 1 : -1;
    Serial.println("goto");
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
    return EEPROM.read(CONFIG_ADDR) == CONFIG_FLAG;
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