#ifndef _ROUTINE_CONFIG_H_
#define _ROUTINE_CONFIG_H_

#define SECS_SETPOINT_ADDR 1019        // Endereço de memória para armazenar o setpoint de segundos
#define SECS_SETPOINT_FLAG_ADDR 1018   // Endereço de memória para verificar se o setpoint foi configurado
#define SECS_SETPOINT_FLAG 43          // Valor para verificar se o setpoint foi configurado
int secondsToWait;                     // Segundos para aguardar entre as rotinas
ServoControl servo;                    // Controle do servo motor


/**
 * Configura o tempo de espera entre as rotinas
 * @param secondsSetpoint segundos para aguardar entre as rotinas
 */
void routineTimeConfig(uint8_t secondsSetpoint) {
  uint8_t minTime = (5000 / servo.getSpeed()) * (servo.getMaxPos() - servo.getMinPos()) / 1000;
  secondsToWait = minTime > secondsSetpoint ? 0 : secondsSetpoint - minTime;
  EEPROM.write(SECS_SETPOINT_ADDR, secondsSetpoint);
}

/**
 * Configuração inicial da rotina
 * - Carrega o setpoint de segundos
 * - Configura o servo motor
 * - Verifica se o setpoint foi configurado
 * @param force_config força configuração do servo
 */
void setupRoutine(bool force_config=false) {
  secondsToWait = EEPROM.read(SECS_SETPOINT_ADDR);
  if (!servo.isConfig() || force_config) {
    servo.config(120, 92, 0);
  }
  if(EEPROM.read(SECS_SETPOINT_FLAG_ADDR) != SECS_SETPOINT_FLAG) {
    routineTimeConfig(7);
    EEPROM.write(SECS_SETPOINT_FLAG_ADDR, SECS_SETPOINT_FLAG);
  }
  servo.begin(8);
}

#endif