/* 
 * File:   scheduling_service.h
 * Author: Apostolos D. Masiakos
 *
 * Created on March 8, 2016, 9:05 PM
 */

#ifndef SCHEDULING_SERVICE_H
#define SCHEDULING_SERVICE_H

#define SCHEDULING_SERVICE_V 0.1

#include "tc_tm.h"

/*declares the maximum available space for on-memory loaded schedule commands*/
#define MAX_STORED_SCHEDULES 4

/**/
typedef enum {
    ALL=0, SUBSET
} ScheduleOpts;

typedef enum {
    /*The 'release_time' member
     * specified on the Scheduling_pck is absolute to OBC time.*/
    ABSOLUTE=0, 
    /*The 'release_time' member
     * specified on the Scheduling_pck is relative to the schedule's
     * activation time.*/
    SCHEDULE=1, 
    /*The 'release_time' member
     * specified on the Scheduling_pck is relative to the sub-schedule's
     * activation time.*/
    SUBSCHEDULE=2, 
    /*The 'release_time' member
     * specified on the Scheduling_pck is relative to the notification time
     * of success of failure of interlocked schedule.
     *  time.*/
    INTERLOCK=3
}Event_Type;
 
/* (Page 105-106 of ECSS-E-70-41A document)
 * Schedule command structure:
 */
typedef struct {
    
        /*Currently not supported by this implementation.*
         *For this specific implementation is set to 1 (one)
         *for every schedule packet.
         */
    uint8_t sub_schedule_id;
    
        /*Number of telecommands in the schedule.
         *For this specific implementation is set to 1 (one) Telecommand
         *per Schedule.
         */
    uint8_t num_of_sche_tel;
    
        /*The number of interlock id to be set by this telecommand.
         *For this specific implementation is set to 0 (zero)
         *for every schedule packet.
         */
    uint8_t intrlck_set_id ;
    
        /*The number of interlock that this telecommand is dependent on.
         *For this specific implementation is set to 0 (zero)
         *for every schedule packet.
         */
    uint8_t intrlck_ass_id ;
    
        /*Success or failure of the dependent telecommand, Success=1, Failure=0
         *For this specific implementation is set to 1 (success)
         *for every schedule packet.
         */
    uint8_t assmnt_type;
    
        /*Determines the release type for the telecommand.*/
    Event_Type schdl_envt;
    
        /*Absolute or relative time of telecommand execution,
         * this field has meaning relative to schdl_envt member.
         */
    uint64_t release_time;
    
        /*This is a delta time which when added to the release time of the scheduled telecommand, the command
         *is expected to complete execution.
         *Timeout execution is only set if telecommand sets interlocks, so for our
         * current implementation will be always 0 (zero)
         */
    uint64_t timeout;
    
        /*The actual telecommand packet to be scheduled and executed*/
    tc_tm_pkt telecmd_pck;
}Schedule_pck;

typedef enum{
    /*Schedule array is full*/
    SCHEDULE_FULL=0,
    /*Subschedule ID invalid*/
    SCH_ID_INVALID,
    /*Interlock ID invalid*/
    INTRL_ID_INVALID,
    DEST_APID_INVALID,
    TIME_INVALID,
    TIME_SPEC_INVALID,
    /*The release time of telecommand is in the execution window
     *of its interlocking telecommand.
     */
    INTRL_LOGIC_ERROR,
    NO_ERROR            
    /*see the 'h' error on page 107*/
}Scheduling_Errors;

extern Schedule_pck mem_sche[MAX_STORED_SCHEDULES];

/*Enables / Disables the scheduling execution
 *Enable state = 1 
 */
OBC_returnStateTypedef enable_schedule( ScheduleOpts opt, uint8_t state  );

/*Service initialization, and runtime*/
TaskFunction_t init_and_run_schedules(void*p);

void update_system_timers();

#endif /* SCHEDULING_SERVICE_H */

