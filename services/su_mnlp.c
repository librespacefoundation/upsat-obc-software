#include "su_mnlp.h"

#undef __FILE_ID__
#define __FILE_ID__ 3

mnlp_response_science_header flight_data;

science_unit_script_inst su_scripts[SU_MAX_SCRIPTS_POPU];
/*the state of the science unit*/
SU_state su_state;
SAT_returnState call_state;
MS_sid active_script;
/*points to the current start byte of a time's table begining, into the loaded script*/
uint16_t current_tt_pointer;
/*points to the next start byte of a time's table begining, into the loaded script*/
uint16_t next_tt_pointer;
/*points to the current start byte of a script's sequence begining, into the loaded script*/
uint16_t current_ss_pointer;
/*points to the next start byte of a time's table begining, into the loaded script*/
uint16_t next_ss_pointer;

uint8_t active_script_sequence;
uint8_t active_script_sequence_command;

SAT_returnState su_incoming_rx() {

    SAT_returnState res;    
    uint8_t c = 0;

    res = HAL_su_uart_rx(&c);
    if( res == SATR_OK ) {
        //if(obc_su_scripts.rx_cnt < SU_SCI_HEADER + 5) { obc_su_scripts.rx_buf[obc_su_scripts.rx_cnt++] = c; }
        //if(obc_su_scripts.rx_cnt < SU_SCI_HEADER + SU_RSP_PCKT_SIZE) { obc_su_scripts.rx_buf[obc_su_scripts.rx_cnt++] = c; }
       // else {
//            su_scripts.rx_cnt = SU_SCI_HEADER;
//            su_scripts.timeout = time_now();
//            if(su_scripts.rx_buf[SU_SCI_HEADER] == SU_ERR_RSP_ID) { su_power_ctrl(P_RESET); }
//
//            /*science header*/
//            cnv32_8(time_now(), &su_scripts.rx_buf[0]);
//            cnv16_8(flight_data.roll, &su_scripts.rx_buf[4]);
//            cnv16_8(flight_data.pitch, &su_scripts.rx_buf[6]);
//            cnv16_8(flight_data.yaw, &su_scripts.rx_buf[8]);
//            cnv16_8(flight_data.roll_dot, &su_scripts.rx_buf[10]);
//            cnv16_8(flight_data.pitch_dot, &su_scripts.rx_buf[12]);
//            cnv16_8(flight_data.yaw_dot, &su_scripts.rx_buf[14]);
//            cnv16_8(flight_data.x_eci, &su_scripts.rx_buf[16]);
//            cnv16_8(flight_data.y_eci, &su_scripts.rx_buf[18]);
//            cnv16_8(flight_data.z_eci, &su_scripts.rx_buf[20]);
//
//            uint16_t size = SU_MAX_RSP_SIZE;
//            mass_storage_storeLogs(SU_LOG, su_scripts.rx_buf, &size);
        }
   // }
    return SATR_OK;
}

uint8_t time_lala = 5; //keep for fun and profit

void su_INIT(){

    su_state = su_off;
    su_load_scripts();
    for (MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
        su_populate_header( &(su_scripts[(uint8_t) i - 1].scr_header), su_scripts[(uint8_t) i - 1].file_load_buf);
        
        /*sort the scripts by increasing T_STARTTIME field (?)*/
        
//        su_populate_scriptPointers( &obc_su_scripts.scripts[(uint8_t)i-1], obc_su_scripts.temp_buf);
        continue;
    }
}

void su_load_scripts() {
    
    for (MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
        /*mark every script as non-valid*/
        su_scripts[(uint8_t) i-1].valid = false;
        /*mark every script as non-active*/
        su_scripts[(uint8_t) i-1].active = false;
        /*load scripts on memory but, parse them at a later time, in order to unlock access to the storage medium for other users*/
        SAT_returnState res = mass_storage_su_load_api(i, su_scripts[(uint8_t) i - 1].file_load_buf);
        if (res == SATR_ERROR || res == SATR_CRC_ERROR) {
            //su_scripts[(uint8_t)i-1].valid = false;
            continue;
        } else {
            /*mark the script as valid, to be parsed afterwards*/
            su_scripts[(uint8_t) i-1].valid = true;
        }
    }
}

void su_SCH() {

    if (su_state == su_off || su_state == su_idle) {

        for (MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
            
            if (su_scripts[(uint8_t) i-1].valid == true && 
                su_scripts[(uint8_t) i-1].scr_header.start_time >= time_lala &&
                su_scripts[(uint8_t) i-1].scr_header.start_time != 0) {

                //TODO: check here on non-volatile mem that we are not executing the same script on the same day,
                //due to a reset.
                //if a reset occur when we have executed a script from start to end, then on the next boot,
                //we will execute it again, we don't want that.
                su_state = su_running;
                active_script = (MS_sid) i;
                
                /* the first byte after the 12-byte sized script header,
                 * points to the first time table, on the active_script script.
                 */
                current_tt_pointer = SU_TT_OFFSET;
                /*finds the next tt that needs to be executed, it iterates all the tt to find the correct one*/
                for (uint16_t b = current_tt_pointer; 
                              b < SU_MAX_FILE_SIZE; b++) { //TODO: check until when for will run

                    call_state = 
                    polulate_next_time_table( su_scripts[(uint8_t) active_script - 1].file_load_buf, 
                                              &su_scripts[(uint8_t) active_script - 1].tt_header,
                                              &current_tt_pointer);
                    if( call_state == SATR_EOT){
                        /*reached the last time table, go for another script, or this script, but on the NEXT day*/
                        //TODO: here to save on non-volatile mem that we have finished executing all ss'es, 
                        //see also line: 150,151
                        break;
                    }
                    else{
                        /*find the script sequence pointed by *time_table->script_index, and execute it */
                        /*start every search after current_tt_pointer, current_tt_pointer now points to the start of the next tt_header*/
                        current_ss_pointer = current_tt_pointer;
                        su_goto_script_seq( &current_ss_pointer, 
                                            &su_scripts[(uint8_t) active_script - 1].tt_header.script_index);                        
                        su_next_cmd( su_scripts[(uint8_t) active_script - 1].file_load_buf, 
                                     &su_scripts[(uint8_t) active_script - 1].seq_header,
                                     &current_ss_pointer);
                        su_cmd_handler( &su_scripts[(uint8_t) active_script - 1].seq_header  );
//                        break;
                    }
//                    if (su_scripts.tt_header.script_index == SU_SCR_TT_EOT) {
//                        break;
//                    }                        // else if(obc_su_scripts.tt_header.time < time_now()) { su_next_tt(obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr); }
//                    else {
//                        break;
//                    }
//                    if (!C_ASSERT(su_scripts.tt_pointer_curr < su_scripts.scripts[i].header.script_len) == true) {
//                        break;
//                    }
                }//script handling for ends here, at this point every time table in the script has been served.
                su_state = su_idle;
                su_scripts[(uint8_t) active_script - 1].active = false;
            }//script validity if ends here
        }//go to check next script
    }
    
//    if (obc_su_scripts.state == su_running) {
//        if (obc_su_scripts.tt_header.time >= time_now()) {
//            if (su_scripts.tt_header.script_index == SU_SCR_TT_EOR) {
//                su_scripts.state = su_finished;
//            } else {
//                SAT_returnState res;
//                su_tt_handler(su_scripts.tt_header, su_scripts.scripts, su_scripts.active_script, &su_scripts.script_pointer_curr);
//                su_next_cmd(su_scripts.active_buf, &su_scripts.cmd_header, &su_scripts.script_pointer_curr);
//
//                for (uint16_t i = su_scripts.script_pointer_curr; i < SU_MAX_FILE_SIZE; i++) {
//
//                    res = su_cmd_handler(&su_scripts.cmd_header);
//                    if (res == SATR_EOT) {
//                        break;
//                    }
//                    su_next_cmd(su_scripts.active_buf, &su_scripts.cmd_header, &su_scripts.script_pointer_curr);
//                }
//                su_next_tt(su_scripts.active_buf, &su_scripts.tt_header, &su_scripts.tt_pointer_curr);
//
//            }
//            // }
//            //}
//            if (time_now() == midnight) {
//                su_INIT();
//                obc_su_scripts.state = su_idle;
//            }
//        }
//    }
}

SAT_returnState su_goto_script_seq(uint16_t *script_sequence_pointer, uint8_t *ss_to_go) {
    
    /*go until the end of the time tables entries*/
    while (su_scripts[(uint8_t) active_script - 1].file_load_buf[(*script_sequence_pointer)++] !=
            /*su_scripts[(uint8_t) active_script - 1].tt_header.script_index*/ SU_SCR_TT_EOT) {
        //                            current_ss_pointer++;
    }/*now current_ss_pointer points to start of S1*/
    if( *ss_to_go == 0x41){ return SATR_OK;}
    for (uint8_t i = 0x41; i <*ss_to_go; i++) {
        while (su_scripts[(uint8_t) active_script - 1].file_load_buf[(*script_sequence_pointer)++] !=
                /*su_scripts[(uint8_t) active_script - 1].tt_header.script_index*/ SU_OBC_EOT_CMD_ID) {
            //                            current_ss_pointer++;
        }/*now current_ss_pointer points to start of S<times>*/
    }
//    /*go until the end of the time tables entries*/
//    while (su_scripts[(uint8_t) active_script - 1].file_load_buf[current_ss_pointer++] !=
//            /*su_scripts[(uint8_t) active_script - 1].tt_header.script_index*/ SU_SCR_TT_EOT) {
//        //                            current_ss_pointer++;
//    }/*now current_ss_pointer points to start of S1*/
//    if( *ss_to_go == 0x41){ return SATR_OK;}
//    for (uint8_t i = 0x41; i <=*ss_to_go; i++) {
//        while (su_scripts[(uint8_t) active_script - 1].file_load_buf[current_ss_pointer++] !=
//                /*su_scripts[(uint8_t) active_script - 1].tt_header.script_index*/ SU_OBC_EOT_CMD_ID) {
//            //                            current_ss_pointer++;
//        }/*now current_ss_pointer points to start of S<times>*/
//    }
    
    return SATR_OK;
}
//SAT_returnState su_tt_handler( science_unit_script_times_table tt, su_script *scripts, MS_sid active_script, uint16_t *start) {
//
//    if(tt.script_index == SU_SCR_TT_S1)        { *start = scripts[active_script].script_pointer_start[0]; }
//    else if(tt.script_index == SU_SCR_TT_S2)   { *start = scripts[active_script].script_pointer_start[1]; }
//    else if(tt.script_index == SU_SCR_TT_S3)   { *start = scripts[active_script].script_pointer_start[2]; }
//    else if(tt.script_index == SU_SCR_TT_S4)   { *start = scripts[active_script].script_pointer_start[3]; }
//    else if(tt.script_index == SU_SCR_TT_S5)   { *start = scripts[active_script].script_pointer_start[4]; }
//
//    return SATR_OK;
//}

SAT_returnState su_cmd_handler( science_unit_script_sequence *cmd) {

    HAL_sys_delay( (cmd->dt_min * 60) + cmd->dt_sec); //*1000 to be millisecs
    
    HAL_su_uart_tx( &cmd->pointer, cmd->len);
    
//    if(cmd->cmd_id == SU_OBC_SU_ON_CMD_ID)          { su_power_ctrl(P_ON); } 
//    else if(cmd->cmd_id == SU_OBC_SU_OFF_CMD_ID)    { su_power_ctrl(P_OFF); } 
//    else if(cmd->cmd_id == SU_OBC_EOT_CMD_ID)       { return SATR_EOT; } 
//    else                                            { HAL_su_uart_tx( &cmd->pointer, cmd->len); }

    return SATR_OK;
}

SAT_returnState su_populate_header( science_unit_script_header *su_script_hdr, uint8_t *buf) {

    if(!C_ASSERT(buf != NULL && su_script_hdr != NULL) == true) { return SATR_ERROR; }
    
    union _cnv cnv;
    cnv.cnv8[0] = buf[0];
    cnv.cnv8[1] = buf[1];
    su_script_hdr->script_len = cnv.cnv16[0];

    cnv.cnv8[0] = buf[2];
    cnv.cnv8[1] = buf[3];
    cnv.cnv8[2] = buf[4];
    cnv.cnv8[3] = buf[5];
    su_script_hdr->start_time = cnv.cnv32;
    
    cnv.cnv8[0] = buf[6];
    cnv.cnv8[1] = buf[7];
    cnv.cnv8[2] = buf[8];
    cnv.cnv8[3] = buf[9];
    su_script_hdr->file_sn = cnv.cnv32;

    su_script_hdr->sw_ver = 0x1F & buf[10];    
    su_script_hdr->su_id = 0x03 & (buf[10] >> 5); //need to check this, seems ok
    su_script_hdr->script_type = 0x1F & buf[11];
    su_script_hdr->su_md = 0x03 & (buf[11] >> 5); //need to check this, seems ok

    cnv.cnv8[0] = buf[su_script_hdr->script_len-2]; /*to check?*/
    cnv.cnv8[1] = buf[su_script_hdr->script_len-1]; /*to check?*/
    su_script_hdr->xsum = cnv.cnv16[0];

    return SATR_OK;
}  
                                            
                                            /*a script , a temp buffer 2K*/
//SAT_returnState su_populate_scriptPointers( su_script *su_scr, uint8_t *buf) {
//
//    /*pointer to the first byte of a times-tables*/
//    uint16_t time_table_ptr = 0;
//    time_table_ptr = SU_TT_OFFSET;
//
//    science_unit_script_time_table temp_tt_header;
//    su_scr->header.scr_sequences = 0;
//    
//    for(uint16_t i = SU_TT_OFFSET; i < SU_MAX_FILE_SIZE; i++) {
//      
//                  /*temp buffer 2K, the tt_header_to fill, the pointer to the time_table*/
//        su_next_tt(buf, &temp_tt_header, &time_table_ptr); /*fill next time table structure*/
//        
//        if(temp_tt_header.script_index == SU_SCR_TT_EOT) { break; } /*EOT, 0x55 found*/
//        if(!C_ASSERT(time_table_ptr < su_scr->header.script_len) == true) { break; } /**/
//    }
//
//    su_scr->script_pointer_start[0] = time_table_ptr;
//    uint16_t script_temp = su_scr->script_pointer_start[0];
//
//    for(uint8_t a = 1; a < SU_CMD_SEQ; a++) {
//
//        if(script_temp == su_scr->header.script_len - 2) { break; }
//
//        for(uint16_t b = 0; b < SU_MAX_FILE_SIZE; b++) {
//            science_unit_script_sequence temp_cmd_header = {0}; /*initialize all struct elements to zero*/
//            if(su_next_cmd(buf, &temp_cmd_header, &script_temp) == SATR_ERROR) { break; };
//            if(temp_cmd_header.cmd_id == SU_OBC_EOT_CMD_ID && script_temp != su_scr->header.script_len - 2) { 
//                su_scr->script_pointer_start[a] = script_temp;
//                su_scr->header.scr_sequences++;
//                break; 
//            } else if(script_temp == su_scr->header.script_len - 2) { break; }
//        }
//        
//    }
//    if(!C_ASSERT(su_scr->header.scr_sequences < SU_CMD_SEQ) == true) { return SATR_ERROR; }
//
//    return SATR_OK;
//}

SAT_returnState polulate_next_time_table( uint8_t *file_buffer, science_unit_script_time_table *time_table, uint16_t *tt_pointer) {

    if(!C_ASSERT(file_buffer != NULL && time_table != NULL && tt_pointer != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(time_table->script_index != SU_SCR_TT_EOT) == true) { return SATR_EOT; }
    
    time_table->sec = file_buffer[(*tt_pointer)++];
    time_table->min = file_buffer[(*tt_pointer)++];
    time_table->hours = file_buffer[(*tt_pointer)++];
    time_table->script_index = file_buffer[(*tt_pointer)++];
    
    if(!C_ASSERT(time_table->sec < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(time_table->min < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(time_table->hours < 23) == true) { return SATR_ERROR; }
    if(!C_ASSERT(time_table->script_index == SU_SCR_TT_S1 || \
                 time_table->script_index == SU_SCR_TT_S2 || \
                 time_table->script_index == SU_SCR_TT_S3 || \
                 time_table->script_index == SU_SCR_TT_S4 || \
                 time_table->script_index == SU_SCR_TT_S5 || \
                 time_table->script_index == SU_SCR_TT_EOT) == true) { return SATR_ERROR; }

    return SATR_OK;
}

SAT_returnState su_next_cmd(uint8_t *file_buffer, science_unit_script_sequence *script_sequence, uint16_t *ss_pointer) {

    if(!C_ASSERT(file_buffer != NULL && script_sequence != NULL && ss_pointer != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(script_sequence->cmd_id != SU_OBC_EOT_CMD_ID) == true) { return SATR_EOT; }

    script_sequence->dt_sec = file_buffer[(*ss_pointer)++];
    script_sequence->dt_min = file_buffer[(*ss_pointer)++];
    script_sequence->cmd_id = file_buffer[(*ss_pointer)++];
    script_sequence->pointer = *ss_pointer;
    script_sequence->len = file_buffer[(*ss_pointer)++];
    script_sequence->seq_cnt = file_buffer[(*ss_pointer)++];

    for(uint8_t i = 1; i < script_sequence->len; i++) {
        script_sequence->parameters[i] = file_buffer[(*ss_pointer)++]; 
    }

    if(!C_ASSERT(script_sequence->dt_sec < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(script_sequence->dt_min < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(script_sequence->cmd_id == SU_OBC_SU_ON_CMD_ID || \
                 script_sequence->cmd_id == SU_OBC_SU_OFF_CMD_ID || \
                 script_sequence->cmd_id == SU_RESET_CMD_ID || \
                 script_sequence->cmd_id == SU_LDP_CMD_ID || \
                 script_sequence->cmd_id == SU_HC_CMD_ID || \
                 script_sequence->cmd_id == SU_CAL_CMD_ID || \
                 script_sequence->cmd_id == SU_SCI_CMD_ID || \
                 script_sequence->cmd_id == SU_HK_CMD_ID || \
                 script_sequence->cmd_id == SU_STM_CMD_ID || \
                 script_sequence->cmd_id == SU_DUMP_CMD_ID || \
                 script_sequence->cmd_id == SU_BIAS_ON_CMD_ID || \
                 script_sequence->cmd_id == SU_BIAS_OFF_CMD_ID || \
                 script_sequence->cmd_id == SU_MTEE_ON_CMD_ID || \
                 script_sequence->cmd_id == SU_MTEE_OFF_CMD_ID || \
                 script_sequence->cmd_id == SU_OBC_EOT_CMD_ID) == true) { return SATR_ERROR; }

    return SATR_OK;
}

SAT_returnState su_power_ctrl(FM_fun_id fid) {

    tc_tm_pkt *temp_pkt = 0;

    function_management_pctrl_crt_pkt_api(&temp_pkt, EPS_APP_ID, fid, SU_DEV_ID);
    if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

    route_pkt(temp_pkt);

    return SATR_OK;
}

//void su_timeout_handler(uint8_t error) {
//    
//    //cnv32_8(time_now(), &obc_su_scripts.rx_buf[0]);
//    //cnv16_8(flight_data.roll, &obc_su_scripts.rx_buf[4]);
//    //cnv16_8(flight_data.pitch, &obc_su_scripts.rx_buf[6]);
//    //cnv16_8(flight_data.yaw, &obc_su_scripts.rx_buf[8]);
//    //cnv16_8(flight_data.roll_dot, &obc_su_scripts.rx_buf[10]);
//    //cnv16_8(flight_data.pitch_dot, &obc_su_scripts.rx_buf[12]);
//    //cnv16_8(flight_data.yaw_dot, &obc_su_scripts.rx_buf[14]);
//    //cnv16_8(flight_data.x_eci, &obc_su_scripts.rx_buf[16]);
//    //cnv16_8(flight_data.y_eci, &obc_su_scripts.rx_buf[18]);
//    //cnv16_8(flight_data.z_eci, &obc_su_scripts.rx_buf[20]);
//
//    uint16_t buf_pointer = SU_SCI_HEADER;
//
//    su_scripts.rx_buf[buf_pointer++] = OBC_SU_ERR_RSP_ID;
//    buf_pointer++;
//    buf_pointer++;
//
//    su_scripts.rx_buf[buf_pointer++] = su_scripts.scripts[su_scripts.active_script].header.xsum;
//    cnv32_8(su_scripts.scripts[su_scripts.active_script].header.start_time, &su_scripts.rx_buf[buf_pointer++]);
//    buf_pointer += 4;
//    cnv32_8(su_scripts.scripts[su_scripts.active_script].header.file_sn, &su_scripts.rx_buf[buf_pointer++]);
//    buf_pointer += 4;
//    su_scripts.rx_buf[buf_pointer++] = (su_scripts.scripts[su_scripts.active_script].header.su_id << 5) | su_scripts.scripts[su_scripts.active_script].header.sw_ver;
//    su_scripts.rx_buf[buf_pointer++] = (su_scripts.scripts[su_scripts.active_script].header.su_md << 5) | su_scripts.scripts[su_scripts.active_script].header.script_type;
//
//    for(uint16_t i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
//
//        su_scripts.rx_buf[buf_pointer++] = su_scripts.scripts[i].header.xsum;
//        cnv32_8(su_scripts.scripts[i].header.start_time, &su_scripts.rx_buf[buf_pointer++]);
//        buf_pointer += 4;
//        cnv32_8(su_scripts.scripts[i].header.file_sn, &su_scripts.rx_buf[buf_pointer++]);
//        buf_pointer += 4;
//        su_scripts.rx_buf[buf_pointer++] = (su_scripts.scripts[i].header.su_id << 5) | su_scripts.scripts[i].header.sw_ver;
//        su_scripts.rx_buf[buf_pointer++] = (su_scripts.scripts[i].header.su_md << 5) | su_scripts.scripts[i].header.script_type;
//    }
//
//    for(uint16_t i = 99; i < 173; i++) {
//        su_scripts.rx_buf[i] = 0;
//    }
//
//    uint16_t size = SU_MAX_RSP_SIZE;
//    mass_storage_storeLogs(SU_LOG, su_scripts.rx_buf, &size);
//    su_power_ctrl(P_RESET);
//    su_scripts.timeout = time_now();
//
//    su_next_tt(su_scripts.active_buf, &su_scripts.tt_header, &su_scripts.tt_pointer_curr);
//
//}