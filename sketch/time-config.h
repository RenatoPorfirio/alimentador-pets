#ifndef _TIME_CONFIG_H_
#define _TIME_CONFIG_H_

#include "time-reg-manager.h"
#include "enhanced-time-span.h"
#include "LDE.h"

#include <RTClib.h>

RTC_DS1307 rtc;
TimeRegManager timeRegManager;
LDE<DateTime*> timeQueue;
EnhancedTimeSpan timeRemaining;

void addTime(DateTime& dt);
void setupTime();
String timeDataToString(TimeData& td, bool showSeconds=false);
void timeRemainingUpdate();

void addTime(DateTime& dt) {
  TimeData* reg = timeRegManager.getTimeReg();
  uint8_t timeQnt = timeRegManager.getTimeRegQnt();

  if(timeQnt == TIME_REG_LIMIT) return;
  
  timeRegManager.addTimeData(dt.hour(), dt.minute(), dt.second());
  DateTime now = rtc.now();
  DateTime ndt(now.year(), now.month(), now.day(), dt.hour(), dt.minute(), dt.second());
  timeQueue.pushBack(new DateTime(
    ndt.year(),
    ndt.month(),
    ndt > now ? ndt.day() : ndt.day() + 1,
    ndt.hour(),
    ndt.minute(),
    ndt.second()
  ));
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
    timeQueue.pushBack(new DateTime(
      ndt.year(),
      ndt.month(),
      ndt > now ? ndt.day() : ndt.day() + 1,
      ndt.hour(),
      ndt.minute(),
      ndt.second()
    ));
  }

  if(timeQnt == 0) {
    DateTime ndt(now.year(), now.month(), now.day() + 1);
    addTime(ndt);
  } else if(timeQnt == 1) {
    DateTime ndt(now.year(), now.month(), now.day() + 1, 1);
    addTime(ndt);
  } else if(timeQnt == 2) {
    DateTime ndt(now.year(), now.month(), now.day() + 1, 2);
    addTime(ndt);
  } else if(timeQnt == 3) {
    DateTime ndt(now.year(), now.month(), now.day() + 1, 3);
    addTime(ndt);
  } else if(timeQnt == 4) {
    DateTime ndt(now.year(), now.month(), now.day() + 1, 4);
    addTime(ndt);
  }
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
  DateTime now = rtc.now();
  timeRemaining = EnhancedTimeSpan(*timeQueue.first() - now);
}


#endif