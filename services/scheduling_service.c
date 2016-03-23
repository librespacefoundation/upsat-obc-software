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
#include <stdbool.h>
//#include <time.h>
#include "FreeRTOS.h"
#include "services.h"
#include "scheduling_service.h"
#include "housekeeping_service.h"
#include "portmacro.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "timekeeping.h"

UART_HandleTypeDef Uart2Handle;

/*Variable local to the Scheduling service*/

TickType_t boot_elapsed_ticks;

/*TODO: becomes a 'state' struct oh .h*/
/*Number of loaded schedules*/
uint8_t nmbr_of_ld_sched = 0;
uint8_t schedule_arr_full = 0;

uint8_t find_schedule_pos();

SC_pkt mem_schedule[SC_MAX_STORED_SCHEDULES];

/*  Initiates the scheduling service.
 *  Loads the schedules from persistent storage.
 */
SAT_returnState load_schedules()
{
    SC_pkt sp1,sp2,sp3,sp4;
    
    sp1.num_of_sch_tc=1;
    sp1.intrlck_set_id=0;
    sp1.intrlck_ass_id=0;
    sp1.assmnt_type = 1;
    sp1.release_time = 5;
    sp1.timeout = 0;
    sp1.enabled = 1;
    
    sp2.num_of_sch_tc=1;
    sp2.intrlck_set_id=0;
    sp2.intrlck_ass_id=0;
    sp2.assmnt_type = 1;
    sp2.release_time = 10;
    sp2.timeout = 0;
    sp2.enabled = 1;
    
    sp3.num_of_sch_tc=1;
    sp3.intrlck_set_id=0;
    sp3.intrlck_ass_id=0;
    sp3.assmnt_type = 1;
    sp3.release_time = 15;
    sp3.timeout = 0;
    sp3.enabled = 1;
    
    sp4.num_of_sch_tc=1;
    sp4.intrlck_set_id=0;
    sp4.intrlck_ass_id=0;
    sp4.assmnt_type = 1;
    sp4.release_time = 20;
    sp4.timeout = 0;
    sp4.enabled = 1;
    
    insert_stc_in_scheduleAPI(mem_schedule,&sp1);
    insert_stc_in_scheduleAPI(mem_schedule,&sp2);
    insert_stc_in_scheduleAPI(mem_schedule,&sp3);
    insert_stc_in_scheduleAPI(mem_schedule,&sp4);
}

void cross_schedules(){
    
    for ( int o=0;o<SC_MAX_STORED_SCHEDULES;o++){
//        if (mem_sche[o].time <= seconds ){

//        }
    }
}

/*
 *
 */
TaskFunction_t init_and_run_schedules(void* p){   
    
    load_schedules();
        time_t current_time;

        /* Cross schedules array, 
         * in every pass check if the specific schedule 
         * if enabled,
         *      if it is then check if its relative or absolute and check the time.
         *      if time >= release time, then execute it. (?? what if time has passed?)
         * else !enabled
         *      if time>= release time, then mark it as !valid
         */
        
        while(1){
//        /* Obtain current time. */
//        current_time = time(NULL);
//
//        if (current_time == ((time_t)-1))
//        {
////            printf(stderr, "Failure to obtain the current time.\n");
////            exit(EXIT_FAILURE);
//        }
//
//        /* Convert to local time format. */
//        c_time_string = ctime(&current_time);
//
//        if (c_time_string == NULL)
//        {
////            printf(stderr, "Failure to convert the current time.\n");
////            exit(EXIT_FAILURE);
//        }
//        HAL_UART_Transmit(&Uart2Handle, (uint8_t *)c_time_string, 25,5000);
//        /* Print to stdout. ctime() has already added a terminating newline character. */
////         printf("%s", c_time_string);
////        exit(EXIT_SUCCESS);
//        
////        if ( scheduling_stateAPI() ){
////            cross_schedules();
////        }
         HAL_Delay(100);
    }
}

SAT_returnState scheduling_app(tc_tm_pkt* spacket){
    return SATR_OK;
}

SAT_returnState insert_stc_in_scheduleAPI( SC_pkt* sch_mem_pool, 
                                           SC_pkt* theSchpck ){        
    
    /*check if schedule array is already full*/
//    if ( nmbr_of_ld_sched == (MAX_STORED_SCHEDULES -1) ){
    if ( schedule_arr_full ){  
        /*TODO: Here to create a telemetry saying "i'm full"*/
        return SATR_SCHEDULE_FULL;
    }
    /*Check sub-schedule id*/
    if ( theSchpck->sub_schedule_id !=1 ){
        return SATR_SSCH_ID_INVALID;
    }
    /*Check number of tc in schpck id*/
    if ( theSchpck->num_of_sch_tc !=1 ){
        return SATR_NMR_OF_TC_INVALID;
    }
    /*Check interlock set id*/
    if ( theSchpck->intrlck_set_id !=0  ){
        return SATR_INTRL_ID_INVALID;
    }
    /*Check interlock assessment id*/
    if ( theSchpck->intrlck_ass_id !=1 ){
        return SATR_ASS_INTRL_ID_INVALID;
    }
    /*Check release time type id*/
    if ( theSchpck->sch_evt != ABSOLUTE ){
        return SATR_RLS_TIMET_ID_INVALID;
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
//    if (!C_ASSERT(pos<SC_MAX_STORED_SCHEDULES)==true){
//        //prob on pos.
//        return SATR_ERROR;
//    }
        
    /*Copy the packet into the array*/
//    mem_schedule[pos] = *theSchpck;
    sch_mem_pool[pos] = *theSchpck;
    nmbr_of_ld_sched++;
    if ( nmbr_of_ld_sched == SC_MAX_STORED_SCHEDULES ){
        /*schedule array has become full*/
        schedule_arr_full = true;
        /*TODO: turn 1, 0 to true, false*/
    }
    return SATR_OK;
}

SAT_returnState scheduling_stateAPI(){
//    if (scheduling_enabled){
//        return R_OK;
//    }
//    else{
//        return R_NOK;
//    }
    return (scheduling_enabled ? SATR_OK : SATR_ERROR);
}

//OBC_returnStateTypedef edit_schedule_state(uint8_t state){
//    scheduling_enabled = state;
//}

SAT_returnState remove_stc_from_scheduleAPI( SC_pkt theSchpck ){
    
    return SATR_OK;
} 

SAT_returnState reset_scheduleAPI(SC_pkt* sch_mem_pool){
    uint8_t pos = 0;
    while( pos<SC_MAX_STORED_SCHEDULES ){
        sch_mem_pool[pos++].valid = false;
    }
    return SATR_OK;
}

SAT_returnState time_shift_all_schedulesAPI(SC_pkt* sch_mem_pool, int32_t secs ){
    
    uint8_t pos = 0;
    while( pos<SC_MAX_STORED_SCHEDULES ){
        if (sch_mem_pool[pos].sch_evt == ABSOLUTE ){
            /*convert the secs to utc and add them or remove them from the time field.*/
            
        }
        else
        if(sch_mem_pool[pos].sch_evt == QB50EPC ){
            /*add them or remove them from the time field. Error if */
        }
    }
    
    return SATR_OK;
}

/* Time shifts selected Schedule_pcks on the Schedule 
 * * Service Subtype 7
 */
SAT_returnState time_shift_sel_schedule(SC_pkt* sch_mem_pool, uint8_t apid, uint16_t seqcount ){
    
    uint8_t pos = 0;
    while( pos<SC_MAX_STORED_SCHEDULES ){
//        if( sch_mem_pool[pos].app_id == apid && 
//            sch_mem_pool[pos].seq_count == seqcount)
//        {
//            /*this is the schedule to be timeshifted. shiftit*/
//            if (sch_mem_pool[pos].sch_evt == ABSOLUTE ){
//            /*convert the secs to utc and add them or remove them from the time field.*/
//            
//        }
//            else
//            if(sch_mem_pool[pos].sch_evt == QB50EPC ){
//                /*add them or remove them from the time field. Error if */
//            }
//        }
    }
    return SATR_OK;
}

/* Time shifts selected telecommands over a time period on the Schedule 
 * * Service Subtype 8
 */
SAT_returnState time_shift_sel_scheduleOTP( SC_pkt* theSchpck ){
    
    return SATR_OK;
}

/* Reports detailed info about every telecommand the Schedule 
 * * Service Subtype 16
 */
SAT_returnState report_detailed( SC_pkt theSchpck ){
    
    return SATR_OK;
}


/* Reports detailed info about a subset of telecommands from the Schedule 
 * * Service Subtype 9
 */
SAT_returnState report_detailed_subset( SC_pkt theSchpck ){
    
    return SATR_OK;
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
SAT_returnState report_summary_all( SC_pkt theSchpck ){
    
    return SATR_OK;
}

/* Reports summary info of a subset of telecommands from the Schedule 
 * * Service Subtype 12
 */
SAT_returnState report_summary_subset( SC_pkt theSchpck ){
    
    return SATR_OK;
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
uint8_t find_schedule_pos(SC_pkt* sche_mem_pool)
{
    if ( nmbr_of_ld_sched == 0){
        return 0;
    }
    else{
        uint8_t pos=0;
        while( sche_mem_pool[pos++].valid == 1 ){
//            pos++;
            if (pos >= SC_MAX_STORED_SCHEDULES){
                return SATR_SCHEDULE_FULL;
            }
            else{
                nmbr_of_ld_sched++;
                return --pos;
            }
        }
//        return nmbr_of_ld_sched;
    }
}