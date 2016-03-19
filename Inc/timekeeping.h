/*
 * timekeeping.h
 *
 *  Created on: 31 ��� 2016
 *      Author: nchronas
 */

#ifndef APPLICATION_USER_TIMEKEEPING_H_
#define APPLICATION_USER_TIMEKEEPING_H_

#include <stdio.h>
#include <stdlib.h>
#include "tc_tm.h"

/* Counts the seconds that has 
 * passed from a reset/boot of the uCtrl.
 * This variable is to be updated at the uCtrl's SysTick()
 * interrupt handling function. 
 */
extern volatile uint32_t seconds;

/* Returns the system's seconds from current boot.
 * Max. seconds to be counted: 2^32 = 4294967296.
 */
uint32_t get_seconds_from_last_bootAPI();

/* Sets the system's seconds from current boot.
 * This call is always successful, returns R_OK.
 */
OBC_returnStateTypedef set_seconds_from_last_bootAPI(uint32_t secs);


//struct time_utc {
//	uint8_t day;
//	uint8_t month;
//	uint8_t year;
//	uint8_t hours;
//	uint8_t min;
//	uint8_t sec;
//};
//
//struct time_keeping {
//	uint32_t epoch;
//	uint32_t elapsed;
//	struct time_utc utc;
//
//};



/*calculate uint size, and perform calculations*/
//const epoch_year_to_sec[] = { };
//const epoch_month_to_sec[] = { };

//void update_time();
//
//void set_time( struct time_utc);
//
//void convert_utc_to_epoch( struct time_utc);

#endif /* APPLICATION_USER_TIMEKEEPING_H_ */
