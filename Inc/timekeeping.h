/*
 * timekeeping.h
 *
 *  Created on: 31 …·Ì 2016
 *      Author: nchronas
 */

#ifndef APPLICATION_USER_TIMEKEEPING_H_
#define APPLICATION_USER_TIMEKEEPING_H_

struct time_keeping {
	uint32_t epoch;
	uint32_t elapsed;
	struct time_utc utc;

};

struct time_utc {
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t hours;
	uint8_t min;
	uint8_t sec;
};

/*calculate uint size, and perform calculations*/
const epoch_year_to_sec[] = { };
const epoch_month_to_sec[] = { };

void update_time();

void set_time( struct time_utc);

void convert_utc_to_epoch( struct time_utc);

#endif /* APPLICATION_USER_TIMEKEEPING_H_ */
