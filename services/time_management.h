/*
 * timekeeping.h
 *
 *  Created on: 31 2016
 *      Author: nchronas
 */

#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "services.h"

#define SECONDSFROM00S
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

extern void HAL_obc_setTime(uint8_t hours, uint8_t mins, uint8_t sec);
extern void HAL_obc_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec);

extern void HAL_obc_setDate(uint8_t mon, uint8_t date, uint8_t year);
extern void HAL_obc_getDate(uint8_t *mon, uint8_t *date, uint8_t *year);

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



typedef struct{
    
    uint32_t last_rtc_sync;
    uint32_t last_gps_sync;
    uint32_t last_gnd_sync;
    
}Time_management_state;

//typedef struct{
//    uint8_t tm_sec;       /* seconds, range 0 to 59          */
//    uint8_t tm_min;       /* minutes, range 0 to 59           */
//    uint8_t tm_hour;      /* hours, range 0 to 23             */
//    uint8_t tm_monthday;  /* day of the month, range 1 to 31  */
//    uint8_t tm_month;     /* month, range 0 to 11             */
//    uint16_t tm_year;     /* The number of years since 1970  */
//    uint8_t tm_weeday;    /* day of the week, range 0 to 6    */
//    uint8_t tm_yearday;   /* day in the year, range 0 to 365  */
//    uint8_t tm_isdst;     /* daylight saving time             */
//}OBCTime_Type;

/* Time management service state.
 * 
 */
extern Time_management_state time_s_state;

/* Counts the seconds that has 
 * passed from a reset/boot of the uCtrl.
 * This variable is to be updated at the uCtrl's SysTick()
 * interrupt handling function. 
 */
extern volatile uint32_t boot_seconds;

/* Counts the seconds that has passed from 
 * the reference epoch of QB50 01/01/2000 00:00:00 UTC, 
 * QB50-SYS-1.4.4 [1]) as defined in the requirements document.
 * interrupt handling function. 
 */
extern volatile uint32_t qb50_seconds;

/* Time represented in GMT format.
 * This variable will be updated from RTC
 * on uctrl boot. Only once.
 */
//extern OBCTime_Type obc_gmt_boot_time;

/* Time represented in GMT format.
 * This variable will be updated from RTC
 * on user request.
 */
//extern OBCTime_Type obc_gmt_time;

/* Returns the system's seconds from current boot.
 * Max. seconds to be counted: 2^32 = 4294967296.
 */
uint32_t time_manage_get_last_boot_seconds_api();


/* Sets the system's seconds from current boot.
 * This call is always successful, returns R_OK.
 */
SAT_returnState set_seconds_from_last_bootAPI(uint32_t secs);

/* Calculates the seconds passed from 
 * 01/01/2000 00:00:00 UTC.
 */
//SAT_returnState calculate_qb50_seconds(OBCTime_Type* gmt_time);

#endif /* APPLICATION_USER_TIMEKEEPING_H_ */
