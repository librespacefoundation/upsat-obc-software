/* 
 * File:   scheduling_service.c
 * Author: Apostolos D. Masiakos
 *
 * Created on March 8, 2016, 9:05 PM
 * This is the implementation of scheduling service as is
 * documented at pages 99-118 of ECSS-E-70-41A document.
 * Service Type 11
 */

#include <stdio.h>
#include <stdlib.h>

#include "../Inc/tc_tm.h"
#include "../Inc/scheduling_service.h"
#include "housekeeping_service.h"

/*  Initiates the scheduling service.
 *  Loads the schedules from persistent storage.
 */
void service_init()
{
    
}

/* Inserts a given Schedule_pck on the schedule array
 * Service Subtype 4
 */
OBC_returnStateTypedef insert_at_schedule( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Removes a given Schedule_pck from the schedule array
 * * Service Subtype 5
 */
OBC_returnStateTypedef remove_from_schedule( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Remove Schedule_pck from schedule over a time period (OTP)
 * * Service Subtype 6
 */
OBC_returnStateTypedef remove_from_scheduleOTP( Schedule_pck theSchpck ){
    
    
    return R_OK;
}

/* Time shifts all Schedule_pcks on the Schedule 
 * * Service Subtype 15
 */
OBC_returnStateTypedef time_shift_all_schedule( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Time shifts selected Schedule_pcks on the Schedule 
 * * Service Subtype 7
 */
OBC_returnStateTypedef time_shift_sel_schedule( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Time shifts selected telecommands over a time period on the Schedule 
 * * Service Subtype 8
 */
OBC_returnStateTypedef time_shift_sel_scheduleOTP( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Reports detailed info about every telecommand the Schedule 
 * * Service Subtype 16
 */
OBC_returnStateTypedef report_detailed( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Reports detailed info about a subset of telecommands from the Schedule 
 * * Service Subtype 9
 */
OBC_returnStateTypedef report_detailed_subset( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* 
 * * Service Subtype 10
 *
OBC_returnStateTypedef report_( Schedule_pck theSchpck ){
    
    return R_OK;
}*/

/* Reports summary info of all telecommands from the Schedule 
 * * Service Subtype 17
 */
OBC_returnStateTypedef report_summary_all( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Reports summary info of a subset of telecommands from the Schedule 
 * * Service Subtype 12
 */
OBC_returnStateTypedef report_summary_subset( Schedule_pck theSchpck ){
    
    return R_OK;
}

/* Reports summary info of all telecommands from the Schedule 
 * * Service Subtype 
 *
OBC_returnStateTypedef ( Schedule_pck theSchpck ){
    
    return R_OK;
}*/
