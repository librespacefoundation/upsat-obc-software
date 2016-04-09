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

extern void HAL_obc_setTime(uint8_t hours, uint8_t mins, uint8_t sec);
extern void HAL_obc_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec);

extern void HAL_obc_setDate(uint8_t mon, uint8_t date, uint8_t year);
extern void HAL_obc_getDate(uint8_t *mon, uint8_t *date, uint8_t *year);

extern uint32_t HAL_obc_GetTick();

//ToDo
//  Set assertions everywhere

/*calculate uint size, and perform calculations*/
extern const uint32_t UTC_QB50_YM[MAX_YEAR][13];

extern const uint32_t UTC_QB50_D[32];

extern const uint32_t UTC_QB50_H[25];

void cnv_UTC_QB50(struct time_utc utc, uint32_t *qb);

void set_time_QB50(uint32_t qb);

void set_time_UTC(struct time_utc utc);

void get_time_QB50(uint32_t *qb);

void get_time_UTC(struct time_utc *utc);

uint32_t get_time_ELAPSED();

uint32_t time_cmp_elapsed(uint32_t t1, uint32_t t2);

#endif
