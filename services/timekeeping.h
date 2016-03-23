/*
 * timekeeping.h
 *
 *  Created on: 31 ��� 2016
 *      Author: nchronas
 */

#ifndef TIMEKEEPING_H
#define TIMEKEEPING_H

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include "services.h"

typedef struct{
    uint8_t tm_sec;       /* seconds, range 0 to 59          */
    uint8_t tm_min;       /* minutes, range 0 to 59           */
    uint8_t tm_hour;      /* hours, range 0 to 23             */
    uint8_t tm_monthday;  /* day of the month, range 1 to 31  */
    uint8_t tm_month;     /* month, range 0 to 11             */
    uint16_t tm_year;     /* The number of years since 1970  */
    uint8_t tm_weeday;    /* day of the week, range 0 to 6    */
    uint8_t tm_yearday;   /* day in the year, range 0 to 365  */
    uint8_t tm_isdst;     /* daylight saving time             */
}OBCTime_Type;

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
extern OBCTime_Type obc_gmt_boot_time;

/* Time represented in GMT format.
 * This variable will be updated from RTC
 * on user request.
 */
extern OBCTime_Type obc_gmt_time;

/* Returns the system's seconds from current boot.
 * Max. seconds to be counted: 2^32 = 4294967296.
 */
uint32_t get_seconds_from_last_bootAPI();

/* Service initialization and runtime function 
 * 
 */
//TaskFunction_t init_and_run_time(void*p);

/* Sets the system's seconds from current boot.
 * This call is always successful, returns R_OK.
 */
SAT_returnState set_seconds_from_last_bootAPI(uint32_t secs);

/* Calculates the seconds passed from 
 * 01/01/2000 00:00:00 UTC.
 */
SAT_returnState calculate_qb50_seconds(OBCTime_Type* gmt_time);

#endif /* APPLICATION_USER_TIMEKEEPING_H_ */
