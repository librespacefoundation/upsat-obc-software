/* 
 * File:   scheduling_service.c
 * Author: 
 *
 * Created on March 8, 2016, 9:05 PM
 * This is the implementation of scheduling service as is
 * documented at pages 99-118 of ECSS-E-70-41A document.
 * Service Type 11 
 * (some restrictions apply)
 */

#include "scheduling_service.h"

SAT_returnState copy_inner_tc(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size);
SC_pkt* find_schedule_pos();

//SC_pkt scheduling_mem_array[SC_MAX_STORED_SCHEDULES];
Scheduling_service_state sc_s_state;

/*  Initiates the scheduling service.
 *  Loads the schedules from persistent storage.
 */
SAT_returnState load_schedules()
{
                                                   /*tc data from 13->, appid id 14*/
        uint8_t schedule_data[] = { 1,1,0,0,1, 4 /*QB50 epoch*/ 
                                    ,0,0,0,10, /*time = 666666666*/
                                    0,33, /*execution timeout = 999*/
                                    24,1,192,185,0,10,16,8,1,6,1,0,0,0,13,0,121 }; /*inner tc payload */
        tc_tm_pkt test ;
        test.ack = 1;
        test.app_id = OBC_APP_ID;
        test.data = schedule_data;
        test.dest_id = OBC_APP_ID;
        test.len = 29;
        test.seq_count = 66;
        test.seq_flags = 0b1111;
        test.ser_subtype = 4;
        test.ser_type = 11;
        test.type = 1;
        test.verification_state = SATR_OK;
        
        scheduling_app(&test);
        
//    SC_pkt sp1,sp2,sp3,sp4;
//    
//    sp1.num_of_sch_tc=1;
//    sp1.intrlck_set_id=0;
//    sp1.intrlck_ass_id=0;
//    sp1.assmnt_type = 1;
//    sp1.release_time = 5;
//    sp1.timeout = 0;
//    sp1.enabled = 1;
//    
//    sp2.num_of_sch_tc=1;
//    sp2.intrlck_set_id=0;
//    sp2.intrlck_ass_id=0;
//    sp2.assmnt_type = 1;
//    sp2.release_time = 10;
//    sp2.timeout = 0;
//    sp2.enabled = 1;
//    
//    sp3.num_of_sch_tc=1;
//    sp3.intrlck_set_id=0;
//    sp3.intrlck_ass_id=0;
//    sp3.assmnt_type = 1;
//    sp3.release_time = 15;
//    sp3.timeout = 0;
//    sp3.enabled = 1;
//    
//    sp4.num_of_sch_tc=1;
//    sp4.intrlck_set_id=0;
//    sp4.intrlck_ass_id=0;
//    sp4.assmnt_type = 1;
//    sp4.release_time = 20;
//    sp4.timeout = 0;
//    sp4.enabled = 1;
//    
//    scheduling_insert_api(&sp1);
//    scheduling_insert_api(&sp2);
//    scheduling_insert_api(&sp3);
//    scheduling_insert_api(&sp4);
}

/*
 *
 */
SAT_returnState scheduling_init_service(){
    
    /* Initialize schedules memory.
     * Assign proper location to inner TC of ScheduleTC for its data.
     */
    for(uint8_t s=0;s<SC_MAX_STORED_SCHEDULES;s++){
        schedule_mem_pool.sc_mem_array[s].tc_pck.data 
                         = schedule_mem_pool.innerd_tc_data[s];
        /* Marks every schedule as invalid, so its position
         * can be taken by a request to the Schedule packets pool.
         */
        schedule_mem_pool.sc_mem_array[s].valid = false;
        schedule_mem_pool.sc_mem_array[s].release_time = -1; /*don't run on boot_second zero, normally this should be avoided by checking validity*/
        sc_s_state.nmbr_of_ld_sched=0;
        sc_s_state.schedule_arr_full = false;
    }
    
    /* Load Schedules from storage.
     * 
     */
//    load_schedules();
}

void cross_schedules(){
    
/* Cross schedules array, 
 *  in every pass check if the specific schedule 
 *  if enabled,
 *      if it is then check if its relative or absolute and check the time.
 *      if time >= release time, then execute it. (?? what if time has passed?)
 *  else !enabled
 *      if time>= release time, then mark it as !valid
 */
    
//    while(1){
        uint8_t i=0;
//        uint32_t current_obc_time = obc
//        boot_seconds;
        for(;i<SC_MAX_STORED_SCHEDULES;i++){
            
            if ( schedule_mem_pool.sc_mem_array[i].valid == true &&
                    schedule_mem_pool.sc_mem_array[i].release_time == boot_seconds ){
                route_pkt( &(schedule_mem_pool.sc_mem_array[i].tc_pck));   
                
                schedule_mem_pool.sc_mem_array[i].valid = false;
                sc_s_state.nmbr_of_ld_sched--;
                sc_s_state.schedule_arr_full = false;
            }
//            vTaskDelay(250 / portTICK_RATE_MS);
//            if ( i == SC_MAX_STORED_SCHEDULES-1) { i=0;}
        }
        
//    }
}

SAT_returnState scheduling_app( tc_tm_pkt *spacket){
    
//    uint8_t sch_mem_pool[MAX_EX_PKT_DATA];
    
    /*TODO: add assertions*/
//    uint8_t pos = find_schedule_pos();
    SC_pkt *the_sc_packet = find_schedule_pos();
    
    uint32_t time = 0;
    uint16_t exec_timeout = 0;
    uint8_t tc_data_len = 0;
    uint8_t offset = 12;
    
//    *the_sc_packet.tc_pck.data = sch_mem_pool;
    
    /*extract the scheduling packet from the data pointer*/
    (*the_sc_packet).sub_schedule_id = spacket->data[0];
    (*the_sc_packet).num_of_sch_tc = spacket->data[1];
    (*the_sc_packet).intrlck_set_id = spacket->data[2];
    (*the_sc_packet).intrlck_ass_id = spacket->data[3];
    (*the_sc_packet).assmnt_type = spacket->data[4];
    (*the_sc_packet).sch_evt = (SC_event_time_type)spacket->data[5];    
    /*7,8,9,10th bytes are the time fields, combine them to a uint32_t*/
    time = ( time | spacket->data[6]) << 8;
    time = ( time | spacket->data[7]) << 8;
    time = ( time | spacket->data[8]) << 8;
    time = ( time | spacket->data[9]);
    /*read execution time out fields*/
    exec_timeout = (exec_timeout | spacket->data[10]) << 8;
    exec_timeout = (exec_timeout | spacket->data[11]);
    
    /*extract data from internal TC packet ( app_id )*/
    (*the_sc_packet).app_id = spacket->data[offset+1];
    (*the_sc_packet).seq_count = 
                            (*the_sc_packet).seq_count | (spacket->data[offset+2] >> 2) ;
    (*the_sc_packet).seq_count << 8;
    (*the_sc_packet).seq_count = 
                            (*the_sc_packet).seq_count | (spacket->data[offset+3]) ;
    (*the_sc_packet).release_time = time;
    (*the_sc_packet).timeout = exec_timeout;
    
    (*the_sc_packet).valid = true;
    (*the_sc_packet).enabled = true;
    
    /*copy the internal TC packet for future use*/
    
    /*  spacket is a TC containing 12 bytes of data related to scheduling service.
     *  After those 12 bytes, a 'whole_inner_tc' packet starts.
     *  
     *  The 'whole_inner_tc' offset in the spacket's data payload is: 12 (13th byte).
     *  
     *  The length of the 'whole_inner_tc' is spacket->data - 12 bytes
     *  
     *  Within the 'whole_inner_tc' the lenght of the 'inner' goes for:
     *  16+16+16+32+(spacket->len - 11)+16 bytes.
     */
    copy_inner_tc( &(spacket->data[12]), &((*the_sc_packet).tc_pck), (uint16_t)spacket->len-12 );

//    route_pkt( &(*the_sc_packet).tc_pck);
    
    scheduling_insert_api( the_sc_packet);
    
    return SATR_OK;
}

SAT_returnState copy_inner_tc(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size) {

    uint8_t tmp_crc[2];
    uint8_t ver, dfield_hdr, ccsds_sec_hdr, tc_pus;
    if(!C_ASSERT(buf != NULL && pkt != NULL && pkt->data != NULL) == true)  { return SATR_ERROR; }
//    if(!C_ASSERT(size > MIN_PKT_SIZE && size < MAX_PKT_SIZE) == true)       { return SATR_ERROR; }

    tmp_crc[0] = buf[size - 1];
    checkSum(buf, size-2, &tmp_crc[1]); /* -2 for excluding the checksum bytes*/
    ver = buf[0] >> 5;
    pkt->type = (buf[0] >> 4) & 0x01;
    dfield_hdr = (buf[0] >> 3) & 0x01;

    pkt->app_id = (TC_TM_app_id)buf[1];

    pkt->seq_flags = buf[2] >> 6;

    cnv8_16((uint8_t*)&buf[2], &pkt->seq_count);
    pkt->seq_count &= 0x3FFF;

    cnv8_16((uint8_t*)&buf[4], &pkt->len);

    ccsds_sec_hdr = buf[6] >> 7;

    tc_pus = buf[6] >> 4;

    pkt->ack = 0x04 & buf[6];

    pkt->ser_type = buf[7];
    pkt->ser_subtype = buf[8];
    pkt->dest_id = (TC_TM_app_id)buf[9];

    pkt->verification_state = SATR_PKT_INIT;

    if(!C_ASSERT(pkt->app_id < LAST_APP_ID) == true) {
        pkt->verification_state = SATR_PKT_ILLEGAL_APPID;
        return SATR_PKT_ILLEGAL_APPID; 
    }

    if(!C_ASSERT(pkt->len == size - ECSS_HEADER_SIZE - 1) == true) {
        pkt->verification_state = SATR_PKT_INV_LEN;
        return SATR_PKT_INV_LEN; 
    }
    pkt->len = pkt->len - ECSS_DATA_HEADER_SIZE - ECSS_CRC_SIZE + 1;

    if(!C_ASSERT(tmp_crc[0] == tmp_crc[1]) == true) {
        pkt->verification_state = SATR_PKT_INC_CRC;
        return SATR_PKT_INC_CRC; 
    }

    if(!C_ASSERT(services_verification_TC_TM[pkt->ser_type][pkt->ser_subtype][pkt->type] == 1) == true) { 
        pkt->verification_state = SATR_PKT_ILLEGAL_PKT_TP;
        return SATR_PKT_ILLEGAL_PKT_TP; 
    }

    if(!C_ASSERT(ver == ECSS_VER_NUMBER) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR; 
    }

    if(!C_ASSERT(tc_pus == ECSS_PUS_VER) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(ccsds_sec_hdr == ECSS_SEC_HDR_FIELD_FLG) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(pkt->type == TC || pkt->type == TM) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(dfield_hdr == ECSS_DATA_FIELD_HDR_FLG) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(pkt->ack == TC_ACK_NO || pkt->ack == TC_ACK_ACC) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(pkt->seq_flags == TC_TM_SEQ_SPACKET) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    /*assertion for data size depanding on pkt type*/
    //if(!C_ASSERT(pkt->len == pkt_size[app_id][type][subtype][generic] == true) {
    //    pkt->verification_state = SATR_ERROR;
    //    return SATR_ERROR;
    //}    

    for(int i = 0; i < pkt->len; i++) {
        pkt->data[i] = buf[ECSS_DATA_OFFSET+i];
    }

    return SATR_OK;
}

SAT_returnState scheduling_insert_api( /*SC_pkt* sch_mem_pool, */
                                       SC_pkt* theSchpck ){        
    
//    if ( report ){
//        
//    }
//    else if ( completely_remove_schedule ){
//        
//    }
//    else if ( temporarilly_disable_a_schedule ){
//        
//    }
//    else if ( enable_a_schedule ){
//        
//    }
//    else if ( reset_schedule ){
//        
//    }
//    else if ( insert_a_schedule ){
//        
//    }
//    else if ( get_service_state ){
//        
//    }
//    else if ( reset_schedule ){
//        
//    }
//    else if ( time_shift_all ){
//        
//    }
//    else if ( time_shift_selected ){
//        
//    }
//    else if ( report_summary_all_and_everything_else ){
//        
//    }
    /*check if schedule array is already full*/
    
//    if ( !C_ASSERT(sc_s_state.schedule_arr_full) == true ){  
//        /*TODO: Here to create a telemetry/log saying "I'm full"*/
//        return SATR_SCHEDULE_FULL;
//    }
    
//    uint8_t pos = find_schedule_pos(scheduling_mem_array);
//    if ( !C_ASSERT(pos != SC_MAX_STORED_SCHEDULES) == true){
//        return SATR_SCHEDULE_FULL;
//    }
    
    /*Check sub-schedule id*/
//    if ( C_ASSERT(theSchpck->sub_schedule_id !=1) == true ){
//        return SATR_SSCH_ID_INVALID;
//    }
//    /*Check number of tc in schpck id*/
//    if ( C_ASSERT(theSchpck->num_of_sch_tc !=1) == true ){
//        return SATR_NMR_OF_TC_INVALID;
//    }
//    /*Check interlock set id*/
//    if ( C_ASSERT(theSchpck->intrlck_set_id !=0) == true){
//        return SATR_INTRL_ID_INVALID;
//    }
//    /*Check interlock assessment id*/
//    if ( C_ASSERT(theSchpck->intrlck_ass_id !=1) == true ){
//        return SATR_ASS_INTRL_ID_INVALID;
//    }
//    /*Check release time type id*/
//    if ( (!C_ASSERT(theSchpck->sch_evt != ABSOLUTE) == true) ||
//         (!C_ASSERT(theSchpck->sch_evt != QB50EPC) == true) ){
//        return SATR_RLS_TIMET_ID_INVALID;
//    }
    /*Check time value*/
//    if (   ){
//        return TIME_SPEC_INVALID;
//    }
    /*Check execution time out*/
//    if (  ){
//       return INTRL_LOGIC_ERROR; 
//    }
        
    /*Place the packet into the scheduling array*/
    sc_s_state.nmbr_of_ld_sched++;
    if ( sc_s_state.nmbr_of_ld_sched == SC_MAX_STORED_SCHEDULES ){
        /*schedule array has become full*/
        sc_s_state.schedule_arr_full = true;
        /*TODO: turn 1, 0 to true, false*/
    }
    return SATR_OK;
}

SAT_returnState scheduling_state_api(){

    return (scheduling_enabled ? SATR_OK : SATR_SCHEDULE_DISABLED);
}

SAT_returnState scheduling_remove_schedule_api( /*SC_pkt* sch_mem_pool, */ 
                                                SC_pkt* theSchpck, uint8_t apid, uint16_t seqc ){
    
    uint8_t pos = 0;
//    while( pos<SC_MAX_STORED_SCHEDULES ){
//        if (scheduling_mem_array[pos].app_id == apid && 
//                scheduling_mem_array[pos].seq_count == seqc){
//            scheduling_mem_array[pos].valid = 0;
//            sc_s_state.nmbr_of_ld_sched--;
//            sc_s_state.schedule_arr_full = 0;
//        }
//        pos++;
//    }
    return SATR_OK;
} 

SAT_returnState scheduling_reset_schedule_api(SC_pkt* sch_mem_pool){
    uint8_t pos = 0;
    while( pos<SC_MAX_STORED_SCHEDULES ){
        sch_mem_pool[pos++].valid = false;
    }
    return SATR_OK;
}

SAT_returnState scheduling_time_shift_all_schedules_api(SC_pkt* sch_mem_pool, int32_t secs ){
    
    uint8_t pos = 0;
    while( pos<SC_MAX_STORED_SCHEDULES ){
        if (sch_mem_pool[pos].sch_evt == ABSOLUTE ){
            /*convert the secs to utc and add them or remove them from the time field.*/
            
            /*TODO: timing api*/
        }
        else
        if(sch_mem_pool[pos].sch_evt == QB50EPC ){
            /*add them or remove them from the time field. Error if */
            
            /*TODO: timing api*/
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
        pos++;
    }
    return SATR_OK;
}

/* Find a 'free' (non-valid schedule) position in the Schedule_pck array to
 * insert the Scheduling packet, and return its address.
 */
SC_pkt* find_schedule_pos(/*SC_pkt* sche_mem_pool*/)
{
    for( uint8_t k=0; k<SC_MAX_STORED_SCHEDULES; k++){
        if( schedule_mem_pool.sc_mem_array[k].valid != true){
            return &( schedule_mem_pool.sc_mem_array[k]);
        }
    }
    return NULL;
}

/* Reports summary info of all telecommands from the Schedule 
 * * Service Subtype 
 *
OBC_returnStateTypedef ( Schedule_pck theSchpck ){
    
    return R_OK;
}*/

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

/* Reports summary info of a subset of telecommands from the Schedule 
 * * Service Subtype 12
 */
SAT_returnState report_summary_subset( SC_pkt theSchpck ){
    
    return SATR_OK;
}

/* Reports detailed info about a subset of telecommands from the Schedule 
 * * Service Subtype 9
 */
SAT_returnState report_detailed_subset( SC_pkt theSchpck ){
    
    return SATR_OK;
}