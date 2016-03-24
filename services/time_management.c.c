/* 
 * File:   time_management.c
 * Author: 
 *
 * Created on March 8, 2016, 9:05 PM
 * This is the implementation of Time management service as is
 * documented at pages 95-97 of ECSS-E-70-41A document.
 * Service Type 9 
 * (some restrictions may apply)
 */

#include "time_management.h.h"

volatile uint32_t boot_seconds = 0;
volatile uint32_t qb50_seconds = 0;

OBCTime_Type obc_gmt_boot_time;
OBCTime_Type obc_gmt_time;


uint32_t time_manage_get_last_boot_seconds_api(){
    return boot_seconds;
}

SAT_returnState set_seconds_from_last_bootAPI(uint32_t secs){
    boot_seconds = secs;
    return SATR_OK;
}


SAT_returnState calculate_qb50_seconds(OBCTime_Type* gmt_time){
    
    uint32_t years_from_2000_to_seconds = (gmt_time->tm_year - 2000)*31556926;  /*1 year (365.24 days) 31556926 seconds*/
    uint32_t this_years_month_to_seconds = (gmt_time->tm_month)*2629743;        /*1 month (30.44 days) 2629743 seconds*/
    uint32_t these_days_to_sec = (gmt_time->tm_monthday-1)*86400;               /*1 day 86400 seconds*/
    uint32_t hours_to_secs = (gmt_time->tm_hour)*3600;                          /*1 hour 3600 seconds*/
    uint32_t minutes_to_secs = (gmt_time->tm_min)*60;                           /*1 minute 60 seconds*/
    
    qb50_seconds = years_from_2000_to_seconds+
                    this_years_month_to_seconds+
                    these_days_to_sec+
                    hours_to_secs+
                    minutes_to_secs+ (gmt_time->tm_sec);
    
    return SATR_OK;
}

////seconds from start of current day
//        double double_seconds = Double.parseDouble( this._gps_message_splitted[1].split(":")[1]);
//        
//        int rem;
//        int hours = (int) ( double_seconds / 3600 );
//            rem = (int) (double_seconds % 3600);
//         
//        int minutes = rem / 60 ;
//        rem = rem % 60;
//        int seconds = rem;
//        
//        this._test_cal.set(Calendar.HOUR_OF_DAY, hours);
//        this._test_cal.set(Calendar.MINUTE, minutes);
//        this._test_cal.set(Calendar.SECOND, seconds);
//        
//        String hour = String.format( "%1$TH:%1$TM:%1$TS", _test_cal );
//        this._gps_time_field.setText(hour);
//    }
    
//  Human readable time 	Seconds
//  1 hour                  3600 seconds
//  1 day                   86400 seconds
//  1 week                  604800 seconds
//  1 month   (30.44 days) 	2629743 seconds
//  1 year   (365.24 days)  31556926 seconds


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
