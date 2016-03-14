/* 
 * File:   scheduling_service.h
 * Author: Apostolos D. Masiakos
 *
 * Created on March 8, 2016, 9:05 PM
 */

#ifndef SCHEDULING_SERVICE_H
#define SCHEDULING_SERVICE_H

#include "tc_tm.h"

/*declares the maximum available space for on-memory loaded schedule commands*/
#define MAX_STORED_SCHEDULES 50

/**/
typedef enum {
    ALL=0, SUBSET
} ScheduleOpts;

typedef enum {
    ABSOLUTE=0, SCHEDULE=1, SUBSCHEDULE=2, INTERLOCK=3
}Scheduling_Event;
 
/* (Page 105-106 of ECSS-E-70-41A document)
 * Schedule command structure:
 */
typedef struct {
        /*TOBEREMOVED*/
    char* schedule_name;
    
        /*Currently not supported*/
    uint8_t sub_schedule_id;
    
        /*Number of telecommands in the schedule*/
    uint8_t num_of_sche_tel;
    
        /*the identification of the interlock id to be set by this telecommand, zero for none*/
    uint8_t intrlck_set_id ;
    
        /*the identification of the interlock that this telecommand is dependent on, zero for none*/
    uint8_t intrlck_ass_id ;
    
        /*success or failure of the dependent telecommand, Success=1, Failure=0*/
    uint8_t assmnt_type;
    
        /*determines the 'type' of the release type for the telecommand*/
    Scheduling_Event schdl_envt;
    
        /*Absolute or relative time of telecommand execution, this field has meaning relative to schdl_envt*/
    uint64_t time;
    
        /*This is a delta time which when added to the release time of the scheduled telecommand, the command
         * is expected to complete execution.
         *Timeout execution is only set if telecommand sets interlocks. */
    uint64_t timeout;
    
        /*The actual telecommand packet to be scheduled and executed*/
    tc_tm_pkt telecmd_pck;
}Schedule_pck;

Schedule_pck mem_sche[MAX_STORED_SCHEDULES];

/**/
OBC_returnStateTypedef enable_schedule( ScheduleOpts opt  );

/**/
OBC_returnStateTypedef dd( tc_tm_pkt *pkt );

/*Service initialization, and runtime*/
TaskFunction_t maintain_service_time(void*p);


#endif /* SCHEDULING_SERVICE_H */

