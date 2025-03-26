#ifndef _ENHANCED_TIME_SPAN_H_
#define _ENHANCED_TIME_SPAN_H_

#include <RTClib.h>

/**
 * Classe que extende a classe TimeSpan do RTClib
 * Adiciona um índice para identificação do objeto
 * Adiciona métodos para decrementar o tempo e obter uma string formatada
 * Adiciona um método para obter uma string de debug
 */
class EnhancedTimeSpan : public TimeSpan {
  public:
    int8_t index;

    EnhancedTimeSpan() : TimeSpan(0), index(-1) {}

    EnhancedTimeSpan(int32_t seconds, int8_t idx = -1)
      : TimeSpan(seconds), index(idx) {}

    EnhancedTimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds, int8_t idx = -1)
      : TimeSpan(days, hours, minutes, seconds), index(idx) {}
    
    EnhancedTimeSpan(const TimeSpan& ts, int8_t idx = -1)
      : TimeSpan(ts), index(idx) {}

    void decrease(int32_t seconds) {
      int32_t newSeconds = this->totalseconds() - seconds;
      if (newSeconds < 0) {
        newSeconds = 0;
      }

      *static_cast<TimeSpan*>(this) = TimeSpan(newSeconds);
    }

    String toString(bool showSeconds=true) const {
      int32_t totalSecs = this->totalseconds();
      int32_t absSecs = totalSecs >= 0 ? totalSecs : -totalSecs;

      int8_t hours = (absSecs / 3600) % 24;
      int8_t minutes = (absSecs % 3600) / 60;
      int8_t seconds = absSecs % 60;

      String str;
      str += (hours < 10 ? "0" : "") + String(hours, 10) + "h";
      str += String(":") + (minutes < 10 ? "0" : "") + String(minutes, 10) + "m";
      if(showSeconds) str += String(":") + (seconds < 10 ? "0" : "") + String(seconds, 10) + "s";
      return str;
    }

    String debugString() const {
      String str = toString();
      str += " | Index: " + String(index);
      str += " | Total Seconds: " + String(this->totalseconds());
      return str;
    }
};

#endif
