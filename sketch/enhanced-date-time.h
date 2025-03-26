#ifndef _ENHANCED_DATE_TIME_H_
#define _ENHANCED_DATE_TIME_H_

#include <RTClib.h>

/**
 * Classe que extende a classe DateTime do RTClib
 * Adiciona um índice para identificação do objeto
 */
class EnhancedDateTime : public DateTime {
  public:
    int8_t index;

    EnhancedDateTime(int16_t years, int16_t months, int16_t days, int8_t hours, int8_t minutes, int8_t seconds, int8_t idx = -1)
      : DateTime(years, months, days, hours, minutes, seconds), index(idx) {}
    
    EnhancedDateTime(const DateTime& dt, int8_t idx = -1)
      : DateTime(dt), index(idx) {}
};

#endif
