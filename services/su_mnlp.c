#include "su_mnlp.h"

struct script_handler obc_su_scripts;

/*
draft implementation
*/
void su_err_handler(uint8_t error) {
    if(error == timeout) {
        struct su_mnlp_obc_su_err_pckt err_pckt;
        eps_su_mnlp_pwr(OFF);
        generate_OBC_SU_ERR_pckt(err_pckt);
        delay_sec(60);
        sd_store_pckt(err_pckt);
        eps_su_mnlp_pwr(ON);

    } else if (error == su_err_pckt) {
        eps_su_mnlp_pwr(OFF);
        delay_sec(60);
        eps_su_mnlp_pwr(ON);
    } else {
        ERROR
    }
}

struct science_data pck;
struct OBC_data flight_data;
uint8_t su_rx_buf[SU_RSP_PCKT_SIZE];

void su_rx_callback(char c) {
    static cnt = 0;
    
    if(cnt<SU_RSP_PCKT_SIZE) {
        buf[c] = c;
        cnt++;
    } else {
        reset_su_timeout_timer();
        cnt = 0;
        pck.res_pck.rsp_id = buf[0];
        pck.res_pck.seq_cnt = buf[1]
        for(int i = 0; i < SU_RSP_PCKT_DATA_SIZE; i++) {
            pck.res_pck.data[i] = buf[i+2];
        }
        post_ev(handle_packet);
    }
}

void handle_packet(){
    
    if( pck.res_pck.rsp_id == SU_ERR_RSP_ID) {
        post_ev(su_err_pckt);
    }

    fill_science_header();
    post_ev(sd_log_su_pck);

}

void fill_science_header() {
    pck.header.time_epoch = flight_data.time_epoch ;
    pck.header.roll = flight_data.roll ;
    pck.header.pitch = flight_data.pitch ;
    pck.header.yaw = flight_data.yaw ;
    pck.header.roll_dot = flight_data.roll_dot ;
    pck.header.pitch_dot = flight_data.pitch_dot ;
    pck.header.yaw_dot = flight_data.yaw_dot ;
    pck.header.x_eci = flight_data.x_eci ;
    pck.header.y_eci = flight_data.y_eci ;
    pck.header.z_eci = flight_data.z_eci ;

}

SAT_returnState su_incoming_rx(uint8_t *resp) {


    return SATR_OK;
}

void su_SCH() {

    if(obc_su_scripts.state == idle) {
        for(MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
            if(obc_su_scripts.scripts[i].header.start_time >= OBC_data.time_epoch && scripts[i].start_time != 0) {
                
                obc_su_scripts.state = running
                obc_su_scripts.active_script = i;
                
                mass_storage_su_load_api(i, &obc_su_scripts.active_buf);
                obc_su_scripts.tt_pointer_curr = SU_TT_OFFSET;

                for(uint16_t b = SU_TT_OFFSET; b < MS_MAX_SU_FILE_SIZE; b++) {

                    su_next_tt(&obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr);

                    if(tt_header.script_index == SU_SCR_TT_EOR) { break; }
                    else if(bc_su_scripts.tt_header.time < OBC_data.time_epoch) { su_next_tt(&obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr); }
                    else { break; }
                    if(!C_ASSERT(tt_temp < su_scr.header.script_len) == true) { break; }
                }
            }
        }
    }
    if(obc_su_scripts.state == running) {
        if(obc_su_scripts.tt_header.time >= OBC_data.time_epoch) { 
            if(obc_su_scripts.tt_header.script_index == SU_SCR_TT_EOR) { obc_su_scripts.state = finished; break; }

            SAT_returnState res;
            su_tt_handler(obc_su_scripts.tt_header, &obc_su_scripts.scripts, obc_su_scripts.active_script, &obc_su_scripts.script_pointer_curr);
            su_next_cmd(&obc_su_scripts.active_buf, &obc_su_scripts.cmd_header, &obc_su_scripts.script_pointer_curr);
            
            for(uint16_t i = obc_su_scripts.script_pointer_curr; i < MS_MAX_SU_FILE_SIZE; i++) {

                res = su_cmd_handler(&obc_su_scripts.cmd_header);
                if(res == SATR_EOT) { break; }
                su_next_cmd(&obc_su_scripts.active_buf, &obc_su_scripts.cmd_header, &obc_su_scripts.script_pointer_curr);
            }
            su_next_tt(&obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr); 
        }


    }
    if(time == midnight) {
        su_INIT();
        obc_su_scripts.state = idle;
    }
}

void su_INIT() {
    for(MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
        mass_storage_su_load_api(i, &obc_su_scripts.temp_buf);
        su_populate_header(&obc_su_scripts.scripts[(uint8_t)i].header, &obc_su_scripts.temp_buf);
        su_populate_scriptPointers(&obc_su_scripts.scripts[(uint8_t)i], &obc_su_scripts.temp_buf);       
    }
}

SAT_returnState su_tt_handler(struct script_times_table tt, struct su_script *scripts, MS_sid active_script, uint16_t *start) {

    if(tt.script_index == SU_SCR_TT_S1)        { *start = scripts[active_script].script_pointer_start[0]; }
    else if(tt.script_index == SU_SCR_TT_S2)   { *start = scripts[active_script].script_pointer_start[1]; }
    else if(tt.script_index == SU_SCR_TT_S3)   { *start = scripts[active_script].script_pointer_start[2]; }
    else if(tt.script_index == SU_SCR_TT_S4)   { *start = scripts[active_script].script_pointer_start[3]; }
    else if(tt.script_index == SU_SCR_TT_S5)   { *start = scripts[active_script].script_pointer_start[4]; }

    return SATR_OK;
}

SAT_returnState su_cmd_handler(struct script_seq *cmd) {

    HAL_delay((cmd->dt_min * 60) + cmd->dt_sec);

    if(cmd->cmd_id == SU_OBC_SU_ON_CMD_ID) {

        tc_tm_pkt *temp_pkt = 0;

        fm_pctrl_crt_pkt_api(&temp_pkt, EPS_APP_ID, P_ON, SU_DEV_ID);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);

    } else if(cmd->cmd_id == SU_OBC_SU_OFF_CMD_ID) {
 
        tc_tm_pkt *temp_pkt = 0;

        fm_pctrl_crt_pkt_api(&temp_pkt, EPS_APP_ID, P_OFF, SU_DEV_ID);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);

    } else if(cmd->cmd_id == SU_OBC_EOT_CMD_ID) {
        return SATR_EOT;
    } else {
        HAL_eps_uart_tx(&cmd->pointer, cmd->len);
    }

    return SATR_OK;
}

SAT_returnState su_populate_header(struct script_hdr *hdr, uint8_t *buf) {


    if(!C_ASSERT(buf != NULL) == true)          { return SATR_ERROR; }
    if(!C_ASSERT(sid <= SU_SCRIPT_7) == true)   { return SATR_INV_STORE_ID; }

    cnv8_16(&buf[0], hdr->script_len);
    cnv8_32(&buf[2], hdr->start_time);
    cnv8_32(&buf[6], hdr->file_sn);

    hdr->sw_ver = 0x1F & buf[10];
    hdr->su_id = 0x03 & (buf[10] >> 5); //need to check this
    hdr->script_type = 0x1F & buf[11];
    hdr->su_md = 0x03 & (buf[11] >> 5); //need to check this

    return SATR_OK;
}  

SAT_returnState su_populate_scriptPointers(struct su_script *su_scr, uint8_t *buf) {

    uint16_t tt_temp = 0;

    tt_temp = SU_TT_OFFSET;

    struct script_times_table temp_tt_header;

    for(uint16_t i = SU_TT_OFFSET; i < MS_MAX_SU_FILE_SIZE; i++) {
        su_next_tt(buf, &temp_tt_header, &tt_temp);
        if(tt_header.script_index == SU_SCR_TT_EOR) { break; }
        if(!C_ASSERT(tt_temp < su_scr.header.script_len) == true) { break; }
    }

    struct script_seq 

    su_scr->script_pointer_curr = 0;
    su_scr->script_pointer_start[0] = tt_temp + SU_TT_HEADER_SIZE;
    uint16_t script_temp = su_scr->script_pointer_start[0];

    for(uint8_t a = 1; a < SU_CMD_SEQ; a++) {

        for(uint16_t b = 0; i < MS_MAX_SU_FILE_SIZE; i++) {
            su_next_cmd(buf, &temp_cmd_header, &script_temp);
            if(temp_cmd_header.cmd_id == SU_OBC_EOT_CMD_ID) { break; }
            else if(!C_ASSERT(script_temp < su_scr->script_len) == true) { break; }
        }
        su_scr->script_pointer_start[a] = script_temp + SU_CMD_HEADER_SIZE;
        
    }
    return SATR_OK;
}

SAT_returnState su_next_tt(uint8_t *buf, struct script_times_table *tt, uint16_t pointer) {

    if(!C_ASSERT(buf != NULL && tt != NULL && pointer != NULL) == true) { return SATR_ERROR; }

    tt->sec = buf[(*pointer)++];
    tt->min = buf[(*pointer)++];
    tt->hours = buf[(*pointer)++];
    tt->script_index = buf[(*pointer)++];

    if(!C_ASSERT(tt->sec < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->min < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->hour < 23) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->script_index == SU_SCR_TT_S1 || \
                 tt->script_index == SU_SCR_TT_S2 || \
                 tt->script_index == SU_SCR_TT_S3 || \
                 tt->script_index == SU_SCR_TT_S4 || \
                 tt->script_index == SU_SCR_TT_S5 || \
                 tt->script_index == SU_SCR_TT_EOR) == true) { return SATR_ERROR; }

    return SATR_OK;
}

SAT_returnState su_next_cmd(uint8_t *buf, struct script_seq *cmd, uint16_t pointer) {

    if(!C_ASSERT(buf != NULL && cmd != NULL && pointer != NULL) == true) { return SATR_ERROR; }

    cmd->dt_sec = buf[(*pointer)++];
    cmd->dt_min = buf[(*pointer)++];
    cmd->cmd_id = buf[(*pointer)++];
    cmd->pointer = *pointer;
    cmd->len = buf[(*pointer)++];
    cmd->seq_cnt = buf[(*pointer)++];

    for(uint8_t i = 1; i < cmd->len; i++) {
        cmd->parameters[i] = buf[(*pointer)++]; 
    }

    if(!C_ASSERT(cmd->dt_sec < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(cmd->dt_min < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(cmd->cmd_id == SU_OBC_SU_ON_CMD_ID || \
                 cmd->cmd_id == SU_OBC_SU_OFF_CMD_ID || \
                 cmd->cmd_id == SU_RESET_CMD_ID || \
                 cmd->cmd_id == SU_LDP_CMD_ID || \
                 cmd->cmd_id == SU_HC_CMD_ID || \
                 cmd->cmd_id == SU_CAL_CMD_ID || \
                 cmd->cmd_id == SU_SCI_CMD_ID || \
                 cmd->cmd_id == SU_HK_CMD_ID || \
                 cmd->cmd_id == SU_STM_CMD_ID || \
                 cmd->cmd_id == SU_DUMP_CMD_ID || \
                 cmd->cmd_id == SU_BIAS_ON_CMD_ID || \
                 cmd->cmd_id == SU_BIAS_OFF_CMD_ID || \
                 cmd->cmd_id == SU_MTEE_ON_CMD_ID || \
                 cmd->cmd_id == SU_MTEE_OFF_CMD_ID || \
                 cmd->cmd_id == SU_OBC_EOT_CMD_ID) == true) { return SATR_ERROR; }

    return SATR_OK;
}
