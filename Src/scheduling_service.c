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
#include "FreeRTOS.h"
#include "tc_tm.h"
#include "scheduling_service.h"
#include "housekeeping_service.h"
#include "portmacro.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

/*Variable local to the Scheduling service*/

TickType_t boot_elapsed_ticks;

/*Number of loaded schedules*/
uint8_t nmbr_of_ld_sched = 0;
uint8_t schedule_arr_full = 0;


uint8_t find_schedule_pos();

Schedule_pck mem_schedule[MAX_STORED_SCHEDULES];

/*  Initiates the scheduling service.
 *  Loads the schedules from persistent storage.
 */
OBC_returnStateTypedef load_schedules()
{
    Schedule_pck sp1,sp2,sp3,sp4;
    
    sp1.num_of_sche_tel=1;
    sp1.intrlck_set_id=0;
    sp1.intrlck_ass_id=0;
    sp1.assmnt_type = ABSOLUTE;
    sp1.release_time = 5;
    sp1.timeout = 0;
    sp1.enabled = 1;
    
    sp2.num_of_sche_tel=1;
    sp2.intrlck_set_id=0;
    sp2.intrlck_ass_id=0;
    sp2.assmnt_type = ABSOLUTE;
    sp2.release_time = 10;
    sp2.timeout = 0;
    sp2.enabled = 1;
    
    sp3.num_of_sche_tel=1;
    sp3.intrlck_set_id=0;
    sp3.intrlck_ass_id=0;
    sp3.assmnt_type = ABSOLUTE;
    sp3.release_time = 15;
    sp3.timeout = 0;
    sp3.enabled = 1;
    
    sp4.num_of_sche_tel=1;
    sp4.intrlck_set_id=0;
    sp4.intrlck_ass_id=0;
    sp4.assmnt_type = ABSOLUTE;
    sp4.release_time = 20;
    sp4.timeout = 0;
    sp4.enabled = 1;
    
    insert_stc_in_scheduleAPI(mem_schedule,&sp1);
    insert_stc_in_scheduleAPI(mem_schedule,&sp2);
    insert_stc_in_scheduleAPI(mem_schedule,&sp3);
    insert_stc_in_scheduleAPI(mem_schedule,&sp4);
}

void cross_schedules(){
    
    for ( int o=0;o<MAX_STORED_SCHEDULES;o++){
//        if (mem_sche[o].time <= seconds ){

//        }
    }
}

/*
 *
 */
TaskFunction_t init_and_run_schedules(void* p){   
    
    load_schedules();
    
    while(1){
        
        if ( scheduling_stateAPI() ){
            cross_schedules();
        }
    }
}


OBC_returnStateTypedef insert_stc_in_scheduleAPI( Schedule_pck* sch_mem_pool, 
                                            Schedule_pck* theSchpck ){        
    
    /*check if schedule array is already full*/
//    if ( nmbr_of_ld_sched == (MAX_STORED_SCHEDULES -1) ){
    if ( schedule_arr_full ){  
        /*TODO: Here to create a telemetry saying "i'm full"*/
        return R_SCHEDULE_FULL;
    }
    /*Check sub-schedule id*/
    if ( theSchpck->sub_schedule_id !=1 ){
        return R_SSCH_ID_INVALID;
    }
    /*Check number of tc in schpck id*/
    if ( theSchpck->num_of_sche_tel !=1 ){
        return R_NMR_OF_TC_INVALID;
    }
    /*Check interlock set id*/
    if ( theSchpck->intrlck_set_id !=0  ){
        return R_INTRL_ID_INVALID;
    }
    /*Check interlock assessment id*/
    if ( theSchpck->intrlck_ass_id !=1 ){
        return R_ASS_INTRL_ID_INVALID;
    }
    /*Check release time type id*/
    if ( theSchpck->schdl_envt != ABSOLUTE ){
        return R_RLS_TIMET_ID_INVALID;
    }
    /*Check time value*/
//    if (   ){
//        return TIME_SPEC_INVALID;
//    }
    /*Check execution time out*/
//    if (  ){
//       return INTRL_LOGIC_ERROR; 
//    }
    
    uint8_t pos = find_schedule_pos(sch_mem_pool);
    /*Copy the packet into the array*/
//    mem_schedule[pos] = *theSchpck;
    sch_mem_pool[pos] = *theSchpck;
    nmbr_of_ld_sched++;
    if ( nmbr_of_ld_sched == MAX_STORED_SCHEDULES ){
        /*schedule array has become full*/
        schedule_arr_full = 1;
    }
    return R_OK;
}

OBC_returnStateTypedef scheduling_stateAPI(){
    
    if (scheduling_enabled){
        return R_OK;
    }
    else{
        return R_NOK;
    }
}

//OBC_returnStateTypedef edit_schedule_state(uint8_t state){
//    scheduling_enabled = state;
//}

OBC_returnStateTypedef remove_stc_from_scheduleAPI( Schedule_pck theSchpck ){
    
    return R_OK;
} 

/* Reset the schedule memory pool.
 * Marks every schedule struct as invalid and eligible for allocation.
 * 
 */
OBC_returnStateTypedef reset_scheduleAPI(Schedule_pck* sche_mem_pool){
    uint8_t pos = 0;
    while( pos<MAX_STORED_SCHEDULES ){
        sche_mem_pool[pos++].valid = 0;
    }
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

/* Find an index position in the Schedule_pck array to
 * insert the Scheduling packet.
 */
uint8_t find_schedule_pos(Schedule_pck* sche_mem_pool)
{
    if ( nmbr_of_ld_sched == 0){
        return 0;
    }
    else{
        uint8_t pos=0;
        while( sche_mem_pool[pos++].valid == 1 ){
//            pos++;
            if (pos >= MAX_STORED_SCHEDULES){
                return R_SCHEDULE_FULL;
            }
            else{
                nmbr_of_ld_sched++;
                return --pos;
            }
        }
//        return nmbr_of_ld_sched;
    }
}