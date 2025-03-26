/**
 * Alimentador de Pets Automatizado - Arduino Mega
 * -------------------------------------------------
 * Este código controla um alimentador de pets automatizado
 * utilizando um Arduino Mega, um display OLED da Adafruit,
 * um módulo RTC (Real-Time Clock) e um servo motor.
 * 
 * Funcionalidades principais:
 * - Gerencia horários de alimentação utilizando RTC.
 * - Exibe informações no display OLED.
 * - Controla um servo motor para liberar comida.
 * - Utiliza botões para configuração.
 */

#include "servo-control.h"     // Controle do servo motor
#include "display-control.h"   // Controle do display OLED
#include "button-config.h"     // Configuração dos botões
#include "time-config.h"       // Configuração dos horários
#include "screens/screens.h"   // Configuração das telas
#include "routine-config.h"    // Configuração da rotina

DisplayControl display;

double delta(double lastTime);

/**
 * Configuração inicial do sistema
 * - Inicializa comunicação serial
 * - Configura display, servo e botões
 * - Define a tela inicial do display
 */
void setup() {
  Serial.begin(9600);
  while (!Serial.availableForWrite()) delay(10);
  display.begin();
  setupButtons();
  setupRoutine();
  setupTime();
  setCurrentScreen(new DisplayTimeScreen(&display));
}

/**
 * Loop principal
 * - Atualiza a tela do display
 * - Verifica cliques de botões
 * - Controla o tempo e ativa o servo quando necessário
 */
void loop() {
  static double lastTime = 0;
  double dt = delta(lastTime);
  
  IScreen* currentScreen = getCurrentScreen();
  currentScreen->render();
  ClickButton::checkAllClicks();

  if (dt > 1000) { // Verifica se passou 1 segundo
    timeRemaining.decrease(1);
    if (timeRemaining.totalseconds() <= 0 && timeQueue.size() > 0) {
      EnhancedDateTime* dt = timeQueue.pop();
      servo.goToMax(); // Libera comida
      delay(secondsToWait * 1000);
      servo.goToMin(); // Retorna à posição inicial
      DateTime now = rtc.now();
      timeQueue.push(new EnhancedDateTime(now.year(), now.month(), now.day() + 1, dt->hour(), dt->minute(), dt->second()));
      delete dt;
      timeRemainingUpdate();
    }
    lastTime += dt;
  }
}

/**
 * Calcula o tempo decorrido desde o último tempo dado
 */
double delta(double lastTime) {
  double currentTime = millis();
  return currentTime - lastTime;
}
