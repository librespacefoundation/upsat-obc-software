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

/*Variable local to the Scheduling service*/
TickType_t boot_elapsed_ticks;

static __IO uint32_t uwTick; /*from STM32f4xx_hal.c */
extern UART_HandleTypeDef Uart2Handle; /*from main.c*/
extern Schedule_pck mem_sche[MAX_STORED_SCHEDULES]; /* from this.h*/

OBC_returnStateTypedef load_schedules()
{
    Schedule_pck sp1,sp2,sp3,sp4;
    sp1.schedule_name="LED1T";
    sp1.num_of_sche_tel=1;
    sp1.intrlck_set_id=0;
    sp1.intrlck_ass_id=0;
    sp1.assmnt_type = ABSOLUTE;
    sp1.time = 2000;
    sp1.timeout = 0;
    
    sp2.schedule_name="LED2T";
    sp2.num_of_sche_tel=1;
    sp2.intrlck_set_id=0;
    sp2.intrlck_ass_id=0;
    sp2.assmnt_type = ABSOLUTE;
    sp2.time = 4000;
    sp2.timeout = 0;
    
    sp3.schedule_name="LED3T";
    sp3.num_of_sche_tel=1;
    sp3.intrlck_set_id=0;
    sp3.intrlck_ass_id=0;
    sp3.assmnt_type = ABSOLUTE;
    sp3.time = 6000;
    sp3.timeout = 0;
    
    sp4.schedule_name="LED3T";
    sp4.num_of_sche_tel=1;
    sp4.intrlck_set_id=0;
    sp4.intrlck_ass_id=0;
    sp4.assmnt_type = ABSOLUTE;
    sp4.time = 8000;
    sp4.timeout = 0;
    
    mem_sche[0]=sp1; mem_sche[1]=sp2; mem_sche[2]=sp3; mem_sche[3]=sp4;
    
//    for ( int o=0;o<4;o++)
//    {
//        printf("\n%d--%s",o,mem_sche[o].schedule_name);
//    }
    
}

/*  Initiates the scheduling service.
 *  Loads the schedules from persistent storage.
 */
TaskFunction_t maintain_service_time(void* p)
{   
    while(1){
        update_system_timers();
        load_schedules();
//        HAL_UART_Transmit(&Uart2Handle, (uint8_t *)boot_elapsed_ticks, 1,10);
//        printf("fs:%d\n",boot_elapsed_ticks);
//        printf("fs:%d\n",HAL_GetTick());
//        printf("fs:%d\n",uwTick);
//        HAL_UART_Transmit(&Uart2Handle, uwTick, 2,10);
    }
}

/*updates the local variable from systick
 *plus, other time synchs will be done here.
 */
void update_system_timers()
{
    boot_elapsed_ticks = HAL_GetTick();
}

/* Inserts a given Schedule_pck on the schedule array
 * Service Subtype 4
 */
OBC_returnStateTypedef insert_in_schedule( Schedule_pck theSchpck ){
    
    
    
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
