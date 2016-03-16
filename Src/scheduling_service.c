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

uint8_t find_schedule_pos();
OBC_returnStateTypedef insert_in_schedule( Schedule_pck* theSchpck );

extern __IO uint32_t uwTick; /*from STM32f4xx_hal.c */
//extern __IO uint32_t seconds; /* from stm32f4xx_it.c */

extern UART_HandleTypeDef Uart2Handle; /*from main.c*/
Schedule_pck mem_sche[MAX_STORED_SCHEDULES]; /* from this.h*/

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
    
    sp2.num_of_sche_tel=1;
    sp2.intrlck_set_id=0;
    sp2.intrlck_ass_id=0;
    sp2.assmnt_type = ABSOLUTE;
    sp2.release_time = 10;
    sp2.timeout = 0;
    
    sp3.num_of_sche_tel=1;
    sp3.intrlck_set_id=0;
    sp3.intrlck_ass_id=0;
    sp3.assmnt_type = ABSOLUTE;
    sp3.release_time = 15;
    sp3.timeout = 0;
    
    sp4.num_of_sche_tel=1;
    sp4.intrlck_set_id=0;
    sp4.intrlck_ass_id=0;
    sp4.assmnt_type = ABSOLUTE;
    sp4.release_time = 20;
    sp4.timeout = 0;
    
    insert_in_schedule(&sp1);
    insert_in_schedule(&sp2);
    insert_in_schedule(&sp3);
    insert_in_schedule(&sp4);
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
        update_system_timers();
        if ( scheduling_enabled ){
            cross_schedules();
        }
    }
}

/*updates the local variable from systick
 *plus, other time synchs will be done here.
 */
void update_system_timers(){
    boot_elapsed_ticks = HAL_GetTick();
}

/* Inserts a given Schedule_pck on the schedule array
 * Service Subtype 4
 */
OBC_returnStateTypedef insert_in_schedule( Schedule_pck* theSchpck ){        
    uint8_t pos = find_schedule_pos();
    mem_sche[pos] = *theSchpck;
    nmbr_of_ld_sched++;
    if (nmbr_of_ld_sched >= MAX_STORED_SCHEDULES){
        nmbr_of_ld_sched--;
    }
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

/* Find an index position in the Schedule_pck array to
 * insert the Scheduling packet.
 */
uint8_t find_schedule_pos()
{
    if ( nmbr_of_ld_sched == 0){
        return 0;
    }
    else{
        return nmbr_of_ld_sched;
    }
}