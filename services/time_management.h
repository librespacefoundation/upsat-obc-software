#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

#include <stdint.h>
#include "services.h"

#define MAX_YEAR 21

struct time_utc {
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hours;
    uint8_t min;
    uint8_t sec;
};

struct time_keeping {
    uint32_t epoch;
    uint32_t elapsed;
    struct time_utc utc;

};

/*calculate uint size, and perform calculations*/
extern const uint32_t UTC_QB50[MAX_YEAR][13];

void cnv_UTC_QB50(struct time_utc utc, uint32_t *qb);

void set_time_QB50(uint32_t qb);

void set_time_UTC(struct time_utc utc);

void get_time_QB50(uint32_t *qb);

void get_time_UTC(struct time_utc *utc);

#endif
