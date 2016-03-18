/* 
 * File:   scheduling_service.h
 * Author: Apostolos D. Masiakos
 *
 * Created on March 8, 2016, 9:05 PM
 */

#ifndef SCHEDULING_SERVICE_H
#define SCHEDULING_SERVICE_H

#define SCHEDULING_SERVICE_V 0.1

/* Declares the maximum available space for 
 * on-memory loaded schedule commands
 */
#define MAX_STORED_SCHEDULES 15

#include "tc_tm.h"

/**/
typedef enum {
    ALL=0,
    SUBSET
}Schedule_options_type;

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
}Schedule_time_type;
 
/* (Page 105-106 of ECSS-E-70-41A document)
 * Schedule command structure:
 */
typedef struct {
    
        /* This is the application id that the telecommand it is destined to.
         * This info will be extracted from the encapsulated TC packet.
         */
    uint8_t app_id;
    
        /* This is the sequence count of the telecommand packet.
         * This info will be extracted (?) from the encapsulated TC packet. (?)
         */
    uint8_t seq_count;
    
        /* If the specific schedule command is enabled.
         * Enabled = 1, Disabled = 0.
         */
    uint8_t enabled;
    
        /* Currently not supported by this implementation.*
         * For this specific implementation is set to 1 (one)
         * for every schedule packet.
         */
    uint8_t sub_schedule_id;
    
        /* Number of telecommands in the schedule.
         * For this specific implementation is set to 1 (one) Telecommand
         * per Schedule.
         */
    uint8_t num_of_sche_tel;
    
        /* The number of interlock id to be set by this telecommand.
         * For this specific implementation is set to 0 (zero)
         * for every schedule packet.
         */
    uint8_t intrlck_set_id ;
    
        /* The number of interlock that this telecommand is dependent on.
         * For this specific implementation is set to 0 (zero)
         * for every schedule packet.
         */
    uint8_t intrlck_ass_id ;
    
        /* Success or failure of the dependent telecommand, Success=1, Failure=0
         * For this specific implementation is set to 1 (success)
         * for every schedule packet.
         */
    uint8_t assmnt_type;
    
        /*Determines the release type for the telecommand.*/
    Schedule_time_type schdl_envt;
    
        /* Absolute or relative time of telecommand execution,
         * this field has meaning relative to schdl_envt member.
         */
    uint64_t release_time;
    
        /* This is a delta time which when added to the release time of the scheduled telecommand, the command
         * is expected to complete execution.
         * Timeout execution is only set if telecommand sets interlocks, so for our
         * current implementation will be always 0 (zero)
         */
    uint64_t timeout;
    
        /* The actual telecommand packet to be scheduled and executed
         * 
         */
    tc_tm_pkt telecmd_pck;
        
        /* Declares a schedule valid or invalid.
         * If a schedule is noted as invalid, it can be replaced 
         * by a new one.
         * When a schedule goes for execution, 
         * automatically becomes invalid.
         */
    uint8_t valid;
    
    
    
}Schedule_pck;

typedef enum{
    /*see the 'h' error on page 107*/
    NO_ERROR = 0,
    /*Schedule array is full*/
    SCHEDULE_FULL,
    /*Subschedule ID invalid*/
    SSCH_ID_INVALID,
    /*Number of telecommands invalid*/
    NMR_OF_TC_INVALID,        
    /*Interlock ID invalid*/
    INTRL_ID_INVALID,
    /*Assess Interlock ID invalid*/
    ASS_INTRL_ID_INVALID,
    /*relese time type ID invalid*/
    RLS_TIMET_ID_INVALID,        
    DEST_APID_INVALID,
    TIME_INVALID,
    TIME_SPEC_INVALID,
    /*The release time of telecommand is in the execution window
     *of its interlocking telecommand.
     */
    INTRL_LOGIC_ERROR
}SchedulingAPIStates;

extern Schedule_pck mem_schedule[MAX_STORED_SCHEDULES];

/* Defines the state of the Scheduling service,
 * if enabled the release of TC is running.
 * Enable = 1
 * Disabled = 0
 */
static scheduling_enabled = 1;

/*
 * Returns !NO_ERROR if scheduling is enabled and running.
 * Returns NO_ERROR if scheduling is disabled.  
 */
SchedulingAPIStates scheduling_status();

/* Enables / Disables the scheduling execution as a service.
 * Enable state = 1
 * Disable state = 0
 * Return R_OK, on successful state alteration.
 */
SchedulingAPIStates edit_schedule_state(tc_tm_pkt* spacket);

SchedulingAPIStates reset_schedule(Schedule_pck* sche_mem_pool);

/*Service initialization, and runtime*/
TaskFunction_t init_and_run_schedules(void*p);

void update_system_timers();

#endif /* SCHEDULING_SERVICE_H */

