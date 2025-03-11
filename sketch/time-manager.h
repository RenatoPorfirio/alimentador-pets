#ifndef _TIME_MANAGER_H_
#define _TIME_MANAGER_H_

#include <RTClib.h>
#include <EEPROM.h>

#define TIME_REG_LIMIT 5

static RTC_DS1307 rtc;

typedef struct {
  int8_t hr;
  int8_t min;
  int8_t sec;
  String toString() {
    String str;
    if(hr < 0 || min < 0 || sec < 0) return String("nulltime");
    str.concat(hr > 9 ? "" : "0");
    str.concat(String(hr, 10) + ":");
    str.concat(min > 9 ? "" : "0");
    str.concat(String(min, 10) + ":");
    str.concat(sec > 9 ? "" : "0");
    str.concat(String(sec, 10));
    return str;
  }
} TimeData;

class TimeManager {
  static TimeData nulltimedata;
  uint8_t timeQnt;

  public:
  void begin() {
    if (!rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while(1) delay(10);
    }

    if (!rtc.isrunning()) {
      Serial.println("RTC is NOT running, let's set the time!");
      rtc.adjust(DateTime(2024, 1, 1, 0, 0, 0));
    }
  }

  String dateTimeToString(DateTime& dt) {
    String str;
    int hr = dt.hour();
    int min = dt.minute();
    int sec = dt.second();
    str.concat(hr > 9 ? "" : "0");
    str.concat(String(hr, 10) + ":");
    str.concat(min > 9 ? "" : "0");
    str.concat(String(min, 10) + ":");
    str.concat(sec > 9 ? "" : "0");
    str.concat(String(sec, 10));
    return str;
  }

  DateTime* createDateTime(int hour=0, int minute=0, int second=0, int year=0, int month=0, int day=0) {
    DateTime now = rtc.now();
    return new DateTime(
      year ? year : now.year(),
      month ? month : now.month(),
      day ? day : now.day(),
      hour,
      minute,
      second
    );
  }
};

TimeData TimeManager::nulltimedata = {-1, -1, -1};

#endif