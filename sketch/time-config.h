#ifndef _TIME_CONFIG_H_
#define _TIME_CONFIG_H_

#include "time-reg-manager.h"
#include "enhanced-time-span.h"
#include "LDE.h"

#include <RTClib.h>

RTC_DS1307 rtc;
TimeRegManager timeRegManager;
LDE<DateTime*> timeQueue;

void setupTime() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(2024, 1, 1, 0, 0, 0));
  }

  timeRegManager.begin();
  DateTime now = rtc.now();
  timeQueue.pushBack(new DateTime(now.year(), now.month(), now.day() + 1));
}

String timeDataToString(TimeData& dt) {
  String str;
  str.concat(dt.hr > 9 ? "" : "0");
  str.concat(String(dt.hr, 10) + ":");
  str.concat(dt.min > 9 ? "" : "0");
  str.concat(String(dt.min, 10) + ":");
  str.concat(dt.sec > 9 ? "" : "0");
  str.concat(String(dt.sec, 10));
  return str;
}

#endif