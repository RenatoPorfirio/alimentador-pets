#ifndef _TIME_REG_MANAGER_H_
#define _TIME_REG_MANAGER_H_

#include <EEPROM.h>

#define TIME_REG_LIMIT 5

typedef struct {
  int8_t hr;
  int8_t min;
  int8_t sec;
} TimeData;

class TimeRegManager {
  static TimeData nulltimedata;
  TimeData reg[TIME_REG_LIMIT];
  uint8_t regQnt = 0;

  public:
  void begin() {
    uint8_t timeQnt = EEPROM.read(0);
    if (timeQnt > TIME_REG_LIMIT) {
      timeQnt = 0;
      TimeRegManager::memClear();
    }
    for (uint8_t i = 0; i < timeQnt; i++) {
      TimeData time;
      EEPROM.get(1 + i * sizeof(TimeData), time);
      reg[i] = time;
    }
  }

  static void memClear() {
    Serial.println("clear");
    EEPROM.write(0, 0);
  }

  void addTimeData(uint8_t hour, uint8_t minute, uint8_t second) {
    if(regQnt == TIME_REG_LIMIT) return;
    TimeData time = {hour, minute, second};
    reg[regQnt] = time;
    EEPROM.put(1 + regQnt * sizeof(TimeData), time);
    EEPROM.write(0, ++regQnt);
  }

  void removeTimeData(int8_t index)
  {
    if (regQnt == 0) return;
    TimeData target = reg[index];
    regQnt--;

    for (int8_t i = index; i < regQnt; i++) {
      reg[i] = reg[i+1];
      EEPROM.put(1 + i * sizeof(TimeData), reg[i]);
    }
    EEPROM.write(0, regQnt);
  }

  TimeData* getTimeReg()
  {
    return reg;
  }

  TimeData* getTimeRegCopy()
  {
    TimeData* copy = new TimeData[regQnt];
    for(uint8_t i = 0; i < regQnt; i++) {
      copy[i] = reg[i];
    }
    return copy;
  }

  uint8_t getTimeRegQnt()
  {
    return regQnt;
  }
};

TimeData TimeRegManager::nulltimedata = {-1, -1, -1};

#endif