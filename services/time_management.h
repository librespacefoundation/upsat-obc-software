#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

#include <stdint.h>
#include "services.h"

#define MAX_YEAR 20

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
extern const uint32_t cnv_QB50[MAX_YEAR][12];

void update_time();

void set_time( struct time_utc);

void convert_utc_to_epoch( struct time_utc);

#endif
