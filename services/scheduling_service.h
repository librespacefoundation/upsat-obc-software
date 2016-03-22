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
#define SC_MAX_STORED_SCHEDULES 15

#include "services.h"

/**/
typedef enum {
    ALL=0,
    SUBSET
}SC_options_type;

typedef enum {
    /* The 'release_time' member
     * specified on the Scheduling_pck is absolute to OBC time.*/
    ABSOLUTE=0, 
    /* The 'release_time' member
     * specified on the Scheduling_pck is relative to the schedule's
     * activation time.*/
    SCHEDULE=1, 
    /* The 'release_time' member
     * specified on the Scheduling_pck is relative to the sub-schedule's
     * activation time.*/
    SUBSCHEDULE=2, 
    /* The 'release_time' member
     * specified on the Scheduling_pck is relative to the notification time
     * of success of failure of interlocked schedule.
     *  time.*/
    INTERLOCK=3,
    /* The 'release_time' member
     * specified on the Scheduling_pck is relative to the seconds passed from
     * QB50 epoch (01/01/2000 00:00:00 UTC).
     *  time.*/        
    QB50EPC=4,
    /*
     * 
     */        
    LAST_EVENTTIME=5
            
}SC_event_time_type;
 
/* (Page 105-106 of ECSS-E-70-41A document)
 * Schedule command structure:
 */
typedef struct {
    
        /* This is the application id that the telecommand it is destined to.
         * This info will be extracted from the encapsulated TC packet.
         */
//    TC_TM_app_id app_id;
    
        /* This is the sequence count of the telecommand packet.
         * This info will be extracted (?) from the encapsulated TC packet. (?)
         */
    uint16_t seq_count;
    
        /* If the specific schedule command is enabled.
         * This is to be changed by Subtype1,2 calls.
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
    uint8_t num_of_sch_tc;
    
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
    
        /* Determines the release type for the telecommand.
         * 
         */
    SC_event_time_type sch_evt;
    
        /* Absolute or relative time of telecommand execution,
         * this field has meaning relative to schdl_envt member.
         */
    uint32_t release_time;
    
        /* This is a delta time which when added to the release time of the scheduled telecommand, the command
         * is expected to complete execution.
         * Timeout execution is only set if telecommand sets interlocks, so for our
         * current implementation will be always 0 (zero)
         */
    uint32_t timeout;
    
        /* The actual telecommand packet to be scheduled and executed
         * 
         */
    tc_tm_pkt tc_pck;
        
        /* Declares a schedule valid or invalid.
         * If a schedule is noted as !valid, it can be replaced 
         * by a new one.
         * When a schedule goes for execution, 
         * automatically becomes invalid.
         * Valid=1, Invalid=0
         */
    uint8_t valid;
    
}SC_pkt;

extern SC_pkt mem_schedule[SC_MAX_STORED_SCHEDULES];

/* Defines the state of the Scheduling service,
 * if enabled the release of TC is running.
 * Enable = 1
 * Disabled = 0
 */
static scheduling_enabled = 1;

/* Service initialization and runtime function 
 * 
 */
TaskFunction_t init_and_run_schedules(void*p);

/* To serve as unique entry point to the Service.
 * To be called from route_packet. 
 */
SAT_returnState scheduling_app(tc_tm_pkt* spacket);

/*
 * Returns R_OK if scheduling is enabled and running.
 * Returns R_NOK if scheduling is disabled.  
 */
SAT_returnState scheduling_state_api();

/* Enables / Disables the scheduling execution as a service.
 * Enable state = 1
 * Disable state = 0
 * Return R_OK, on successful state alteration.
 */
SAT_returnState edit_schedule_stateAPI(tc_tm_pkt* spacket);

/* Reset the schedule memory pool.
 * Marks every schedule struct as invalid and eligible for allocation.
 * 
 */
SAT_returnState reset_scheduleAPI(SC_pkt* sche_mem_pool);

/* Inserts a given Schedule_pck on the schedule array
 * Service Subtype 4
 */
SAT_returnState insert_stc_in_scheduleAPI(SC_pkt* sch_mem_pool,
                                          SC_pkt* theSchpck );

/* Removes a given Schedule_pck from the schedule array
 * * Service Subtype 5
 */
SAT_returnState remove_stc_from_scheduleAPI( SC_pkt theSchpck );

/* Remove Schedule_pck from schedule over a time period (OTP)
 * * Service Subtype 6
 */
SAT_returnState remove_from_scheduleOTPAPI( SC_pkt theSchpck );

/* Time shifts all Schedule_pcks on the Schedule.
 * int32_t secs parameter can be positive or negative seconds value.
 * If positive the seconds are added to the Schedule's TC time, 
 * if negative the seconds are substracted from the Schedule's TC time value. 
 * Service Subtype 15.
 */
SAT_returnState time_shift_all_schedulesAPI( SC_pkt* sch_mem_pool, int32_t secs );

#endif /* SCHEDULING_SERVICE_H */

