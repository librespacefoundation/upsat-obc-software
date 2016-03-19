/*
 * timekeeping.c
 *
 *  Created on: 31 ��� 2016
 *      Author: nchronas
 */


#include "timekeeping.h"

volatile uint32_t seconds = 0;

uint32_t get_seconds_from_last_bootAPI(){
    return seconds;
}

OBC_returnStateTypedef set_seconds_from_last_bootAPI(uint32_t secs){
    seconds = secs;
    return R_OK;
}

//void update_time() {
//	obc.time.elapsed++;
//	obc.time.epoch++;
//
//	obc.time.utc.sec++;
//	if(obc.time.utc.sec >= 60) {
//		obc.time.utc.sec = 0;
//		obc.time.utc.min++;
//		if(obc.time.utc.min >= 60) {
//			obc.time.utc.min = 0;
//			if(obc.time.utc.hour >= 24) {
//				obc.time.utc.hour = 0;
//				/*post event for su_cscript handler*/
//			}
//		}
//	}
//}
//
//void set_time( struct time_utc time) {
//	obc.time.utc.day = time.day;
//	obc.time.utc.month = time.month;
//	obc.time.utc.year = time.year;
//
//	obc.time.utc.hour = time.hour;
//	obc.time.utc.min = time.min;
//	obc.time.utc.sec = time.sec;
//
//	convert_utc_to_epoch();
//}
//
//void convert_utc_to_epoch( struct time_utc time) {
//	year = epoch_year_to_sec[2000-time.year];
//	month = epoch_month_to_sec[time.month];
//	day = time.day*86400;
//
//	hour = time.hour*3600;
//	minutes = time.min*80;
//
//	obc.time.epoch = year + month + day + hour + minutes + time.sec;
//}
