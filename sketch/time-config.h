#ifndef _TIME_CONFIG_H_
#define _TIME_CONFIG_H_

#include "time-reg-manager.h"
#include "enhanced-time-span.h"
#include "enhanced-date-time.h"
#include "min-heap.h"

#include <RTClib.h>

/*
 * Modulo de configuração de horários
 * - Utiliza RTC_DS1307 para controle de tempo
 * - Utiliza TimeRegManager para gerenciar horários de alimentação
 * - Utiliza MinHeap para organizar os horários
 * - Utiliza EnhancedTimeSpan para controlar o tempo restante
 * - Utiliza EnhancedDateTime para controlar os horários
 * - Utiliza TimeData para armazenar os horários
 */

RTC_DS1307 rtc;
TimeRegManager timeRegManager;
MinHeap<EnhancedDateTime*> timeQueue;
EnhancedTimeSpan timeRemaining;

/*
  * Adiciona um horário de alimentação
  * - Adiciona o horário no TimeRegManager
  * - Adiciona o horário na fila de horários
  * - Atualiza o tempo restante
  * @param td horário de alimentação
 */
void addTime(TimeData& td);

/*
  * Configuração inicial do módulo de tempo
  * - Inicializa RTC
  * - Carrega horários de alimentação
  * - Adiciona horários na fila
  * - Atualiza o tempo restante
 */
void setupTime();

/*
  * Converte um horário de alimentação em uma string formatada
  * @param td horário de alimentação
  * @param showSeconds exibir segundos
  * @return string formatada
 */
String timeDataToString(TimeData& td, bool showSeconds=false);

/*
  * Atualiza o tempo restante
 */
void timeRemainingUpdate();


void addTime(TimeData& td) {
  TimeData* reg = timeRegManager.getTimeReg();
  uint8_t timeQnt = timeRegManager.getTimeRegQnt();

  if(timeRegManager.isFull()) return;
  
  timeRegManager.addTimeData(td.hr, td.min, td.sec);
  DateTime now = rtc.now();
  DateTime ndt(now.year(), now.month(), now.day(), td.hr, td.min, td.sec);
  DateTime *result = new EnhancedDateTime(
    ndt.year(),
    ndt.month(),
    ndt > now ? ndt.day() : ndt.day() + 1,
    ndt.hour(),
    ndt.minute(),
    ndt.second(),
    timeQnt
  );
  timeQueue.push(result);
  timeRemainingUpdate();
}

void setupTime() {
  TimeData* reg;
  uint8_t timeQnt;

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(2024, 1, 1, 0, 0, 0));
  }

  timeRegManager.begin();
  // timeRegManager.memClear();
  
  reg = timeRegManager.getTimeReg();
  timeQnt = timeRegManager.getTimeRegQnt();
  DateTime now = rtc.now();

  for(uint8_t i = 0; i < timeQnt; i++) {
    DateTime ndt(now.year(), now.month(), now.day(), reg[i].hr, reg[i].min, reg[i].sec);
    timeQueue.push(new EnhancedDateTime(
      ndt.year(),
      ndt.month(),
      ndt > now ? ndt.day() : ndt.day() + 1,
      ndt.hour(),
      ndt.minute(),
      ndt.second(),
      i
    ));
  }

  // if(timeQnt == 0) {
  //   DateTime ndt(now.year(), now.month(), now.day() + 1);
  //   addTime(ndt);
  // } else if(timeQnt == 1) {
  //   DateTime ndt(now.year(), now.month(), now.day() + 1, 1);
  //   addTime(ndt);
  // } else if(timeQnt == 2) {
  //   DateTime ndt(now.year(), now.month(), now.day() + 1, 2);
  //   addTime(ndt);
  // } else if(timeQnt == 3) {
  //   DateTime ndt(now.year(), now.month(), now.day() + 1, 3);
  //   addTime(ndt);
  // } else if(timeQnt == 4) {
  //   DateTime ndt(now.year(), now.month(), now.day() + 1, 4);
  //   addTime(ndt);
  // }
  timeRemainingUpdate();
}


String timeDataToString(TimeData& td, bool showSeconds=false) {
  String str;
  str += (td.hr < 10 ? "0" : "") + String(td.hr, 10) + "h";
  str += String(":") + (td.min < 10 ? "0" : "") + String(td.min, 10) + "m";
  if(showSeconds) str += String(":") + (td.sec < 10 ? "0" : "") + String(td.sec, 10) + "s";
  return str;
}

void timeRemainingUpdate() {
  if(timeRegManager.isEmpty()) {
    timeRemaining = EnhancedTimeSpan();
    return;
  }
  DateTime now = rtc.now();
  timeRemaining = EnhancedTimeSpan(*timeQueue.top() - now);
}

#endif