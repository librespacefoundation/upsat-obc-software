#include "time_management.h"

#undef __FILE_ID__
#define __FILE_ID__ 14

const uint32_t UTC_QB50_YM[MAX_YEAR][13] = {    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                { 0, 31622400, 34300800, 36720000, 39398400, 41990400, 44668800, 47260800, 49939200, 52617600, 55209600, 57888000, 60480000 },
                                                { 0, 63158400, 65836800, 68256000, 70934400, 73526400, 76204800, 78796800, 81475200, 84153600, 86745600, 89424000, 92016000 },
                                                { 0, 94694400, 97372800, 99792000, 102470400, 105062400, 107740800, 110332800, 113011200, 115689600, 118281600, 120960000, 123552000 },
                                                { 0, 126230400, 128908800, 131414400, 134092800, 136684800, 139363200, 141955200, 144633600, 147312000, 149904000, 152582400, 155174400 },
                                                { 0, 157852800, 160531200, 162950400, 165628800, 168220800, 170899200, 173491200, 176169600, 178848000, 181440000, 184118400, 186710400 },
                                                { 0, 189388800, 192067200, 194486400, 197164800, 199756800, 202435200, 205027200, 207705600, 210384000, 212976000, 215654400, 218246400 },
                                                { 0, 220924800, 223603200, 226022400, 228700800, 231292800, 233971200, 236563200, 239241600, 241920000, 244512000, 247190400, 249782400 },
                                                { 0, 252460800, 255139200, 257644800, 260323200, 262915200, 265593600, 268185600, 270864000, 273542400, 276134400, 278812800, 281404800 },
                                                { 0, 284083200, 286761600, 289180800, 291859200, 294451200, 297129600, 299721600, 302400000, 305078400, 307670400, 310348800, 312940800 },
                                                { 0, 315619200, 318297600, 320716800, 323395200, 325987200, 328665600, 331257600, 333936000, 336614400, 339206400, 341884800, 344476800 },
                                                { 0, 347155200, 349833600, 352252800, 354931200, 357523200, 360201600, 362793600, 365472000, 368150400, 370742400, 373420800, 376012800 },
                                                { 0, 378691200, 381369600, 383875200, 386553600, 389145600, 391824000, 394416000, 397094400, 399772800, 402364800, 405043200, 407635200 },
                                                { 0, 410313600, 412992000, 415411200, 418089600, 420681600, 423360000, 425952000, 428630400, 431308800, 433900800, 436579200, 439171200 },
                                                { 0, 441849600, 444528000, 446947200, 449625600, 452217600, 454896000, 457488000, 460166400, 462844800, 465436800, 468115200, 470707200 },
                                                { 0, 473385600, 476064000, 478483200, 481161600, 483753600, 486432000, 489024000, 491702400, 494380800, 496972800, 499651200, 502243200 },
                                                { 0, 504921600, 507600000, 510105600, 512784000, 515376000, 518054400, 520646400, 523324800, 526003200, 528595200, 531273600, 533865600 },
                                                { 0, 536544000, 539222400, 541641600, 544320000, 546912000, 549590400, 552182400, 554860800, 557539200, 560131200, 562809600, 565401600 },
                                                { 0, 568080000, 570758400, 573177600, 575856000, 578448000, 581126400, 583718400, 586396800, 589075200, 591667200, 594345600, 596937600 },
                                                { 0, 599616000, 602294400, 604713600, 607392000, 609984000, 612662400, 615254400, 617932800, 620611200, 623203200, 625881600, 628473600 },
                                                { 0, 631152000, 633830400, 636336000, 639014400, 641606400, 644284800, 646876800, 649555200, 652233600, 654825600, 657504000, 660096000 }
                                            };

const uint32_t UTC_QB50_D[32] = { 0,
                                  86400,
                                  172800,
                                  259200,
                                  345600,
                                  432000,
                                  518400,
                                  604800,
                                  691200,
                                  777600,
                                  864000,
                                  950400,
                                  1036800,
                                  1123200,
                                  1209600,
                                  1296000,
                                  1382400,
                                  1468800,
                                  1555200,
                                  1641600,
                                  1728000,
                                  1814400,
                                  1900800,
                                  1987200,
                                  2073600,
                                  2160000,
                                  2246400,
                                  2332800,
                                  2419200,
                                  2505600,
                                  2592000,
                                  2678400
                                };

const uint32_t UTC_QB50_H[25] = { 0,
                                  3600,
                                  7200,
                                  10800,
                                  14400,
                                  18000,
                                  21600,
                                  25200,
                                  28800,
                                  32400,
                                  36000,
                                  39600,
                                  43200,
                                  46800,
                                  50400,
                                  54000,
                                  57600,
                                  61200,
                                  64800,
                                  68400,
                                  72000,
                                  75600,
                                  79200,
                                  82800,
                                  86400
                                };


void cnv_UTC_QB50(struct time_utc utc, uint32_t *qb) {
    *qb = UTC_QB50_YM[utc.year][utc.month] + UTC_QB50_D[utc.day] + UTC_QB50_H[utc.hours] + utc.min * 60 + utc.sec;  
}

void set_time_QB50(uint32_t qb) {
  
}

void set_time_UTC(struct time_utc utc) {
    HAL_obc_setDate(utc.month, utc.day, utc.year);
    HAL_obc_setTime(utc.hours, utc.min, utc.sec);
}

void get_time_QB50(uint32_t *qb) {

    struct time_utc utc;

    HAL_obc_getDate(&utc.month, &utc.day, &utc.year);
    HAL_obc_getTime(&utc.hours, &utc.min, &utc.sec);
    cnv_UTC_QB50(utc, qb);

}


void get_time_UTC(struct time_utc *utc) {

    HAL_obc_getDate(&utc->month, &utc->day, &utc->year);
    HAL_obc_getTime(&utc->hours, &utc->min, &utc->sec);

}

uint32_t get_time_ELAPSED() {
    return HAL_obc_GetTick();
}

/*works when the tick ovf*/
uint32_t time_cmp_elapsed(uint32_t t1, uint32_t t2) {
    return t2 - t1;
}
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

volatile uint32_t boot_seconds = 0;
volatile uint32_t qb50_seconds = 0;

//OBCTime_Type obc_gmt_boot_time;
//OBCTime_Type obc_gmt_time;

uint32_t time_manage_get_last_boot_seconds_api(){
    return boot_seconds;
}

SAT_returnState set_seconds_from_last_bootAPI(uint32_t secs){
    boot_seconds = secs;
    return SATR_OK;
}

//void cnv_UTC_QB50(struct time_utc utc, uint32_t *qb) {
//    *qb = UTC_QB50_YM[utc.year][utc.month] + UTC_QB50_D[utc.day] + UTC_QB50_H[utc.hours] + utc.min * 60 + utc.sec;  
//}
//
//void set_time_QB50(uint32_t qb) {
//  
//}
//
//void set_time_UTC(struct time_utc utc) {
//    HAL_obc_setDate(utc.month, utc.day, utc.year);
//    HAL_obc_setTime(utc.hours, utc.min, utc.sec);
//}
//
//void get_time_QB50(uint32_t *qb) {
//
//    struct time_utc utc;
//
//    HAL_obc_getDate(&utc.month, &utc.day, &utc.year);
//    HAL_obc_getTime(&utc.hours, &utc.min, &utc.sec);
//    cnv_UTC_QB50(utc, qb);
//
//}
//
//
//void get_time_UTC(struct time_utc *utc) {
//
//    HAL_obc_getDate(&utc->month, &utc->day, &utc->year);
//    HAL_obc_getTime(&utc->hours, &utc->min, &utc->sec);
//
//}


//SAT_returnState calculate_qb50_seconds(OBCTime_Type* gmt_time){
//    
//    uint32_t years_from_2000_to_seconds = (gmt_time->tm_year - 2000)*31556926;  /*1 year (365.24 days) 31556926 seconds*/
//    uint32_t this_years_month_to_seconds = (gmt_time->tm_month)*2629743;        /*1 month (30.44 days) 2629743 seconds*/
//    uint32_t these_days_to_sec = (gmt_time->tm_monthday-1)*86400;               /*1 day 86400 seconds*/
//    uint32_t hours_to_secs = (gmt_time->tm_hour)*3600;                          /*1 hour 3600 seconds*/
//    uint32_t minutes_to_secs = (gmt_time->tm_min)*60;                           /*1 minute 60 seconds*/
//    
//    qb50_seconds = years_from_2000_to_seconds+
//                    this_years_month_to_seconds+
//                    these_days_to_sec+
//                    hours_to_secs+
//                    minutes_to_secs+ (gmt_time->tm_sec);
//    
//    return SATR_OK;
//}

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
