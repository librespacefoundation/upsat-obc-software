#include "su_mnlp.h"

#undef __FILE_ID__
#define __FILE_ID__ 3

struct script_handler obc_su_scripts;
struct OBC_data flight_data;

/*
draft implementation
*/
void su_timeout_handler(uint8_t error) {
    
    cnv32_8(time_now(), &obc_su_scripts.rx_buf[0]);
    cnv16_8(flight_data.roll, &obc_su_scripts.rx_buf[4]);
    cnv16_8(flight_data.pitch, &obc_su_scripts.rx_buf[6]);
    cnv16_8(flight_data.yaw, &obc_su_scripts.rx_buf[8]);
    cnv16_8(flight_data.roll_dot, &obc_su_scripts.rx_buf[10]);
    cnv16_8(flight_data.pitch_dot, &obc_su_scripts.rx_buf[12]);
    cnv16_8(flight_data.yaw_dot, &obc_su_scripts.rx_buf[14]);
    cnv16_8(flight_data.x_eci, &obc_su_scripts.rx_buf[16]);
    cnv16_8(flight_data.y_eci, &obc_su_scripts.rx_buf[18]);
    cnv16_8(flight_data.z_eci, &obc_su_scripts.rx_buf[20]);

    uint16_t buf_pointer = SU_SCI_HEADER;

    obc_su_scripts.rx_buf[buf_pointer++] = OBC_SU_ERR_RSP_ID;
    buf_pointer++;
    buf_pointer++;

    obc_su_scripts.rx_buf[buf_pointer++] = obc_su_scripts.scripts[obc_su_scripts.active_script].header.xsum;
    cnv32_8(obc_su_scripts.scripts[obc_su_scripts.active_script].header.start_time, &obc_su_scripts.rx_buf[buf_pointer++]);
    buf_pointer += 4;
    cnv32_8(obc_su_scripts.scripts[obc_su_scripts.active_script].header.file_sn, &obc_su_scripts.rx_buf[buf_pointer++]);
    buf_pointer += 4;
    obc_su_scripts.rx_buf[buf_pointer++] = (obc_su_scripts.scripts[obc_su_scripts.active_script].header.su_id << 5) | obc_su_scripts.scripts[obc_su_scripts.active_script].header.sw_ver;
    obc_su_scripts.rx_buf[buf_pointer++] = (obc_su_scripts.scripts[obc_su_scripts.active_script].header.su_md << 5) | obc_su_scripts.scripts[obc_su_scripts.active_script].header.script_type;

    for(uint16_t i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {

        obc_su_scripts.rx_buf[buf_pointer++] = obc_su_scripts.scripts[i].header.xsum;
        cnv32_8(obc_su_scripts.scripts[i].header.start_time, &obc_su_scripts.rx_buf[buf_pointer++]);
        buf_pointer += 4;
        cnv32_8(obc_su_scripts.scripts[i].header.file_sn, &obc_su_scripts.rx_buf[buf_pointer++]);
        buf_pointer += 4;
        obc_su_scripts.rx_buf[buf_pointer++] = (obc_su_scripts.scripts[i].header.su_id << 5) | obc_su_scripts.scripts[i].header.sw_ver;
        obc_su_scripts.rx_buf[buf_pointer++] = (obc_su_scripts.scripts[i].header.su_md << 5) | obc_su_scripts.scripts[i].header.script_type;
    }

    for(uint16_t i = 99; i < 173; i++) {
        obc_su_scripts.rx_buf[i] = 0;
    }

    uint16_t size = SU_MAX_RSP_SIZE;
    mass_storage_storeLogs(SU_LOG, obc_su_scripts.rx_buf, &size);
    su_power_ctrl(P_RESET);
    obc_su_scripts.timeout = time_now();

    su_next_tt(obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr);

}

SAT_returnState su_incoming_rx() {

    SAT_returnState res;    
    uint8_t c = 0;

    res = HAL_su_uart_rx(&c);
    if( res == SATR_OK ) {
        //if(obc_su_scripts.rx_cnt < SU_SCI_HEADER + 5) { obc_su_scripts.rx_buf[obc_su_scripts.rx_cnt++] = c; }
        if(obc_su_scripts.rx_cnt < SU_SCI_HEADER + SU_RSP_PCKT_SIZE) { obc_su_scripts.rx_buf[obc_su_scripts.rx_cnt++] = c; }
        else {

            obc_su_scripts.rx_cnt = SU_SCI_HEADER;

            obc_su_scripts.timeout = time_now();
            if(obc_su_scripts.rx_buf[SU_SCI_HEADER] == SU_ERR_RSP_ID) { su_power_ctrl(P_RESET); }

            /**science header*/
            cnv32_8(time_now(), &obc_su_scripts.rx_buf[0]);
            cnv16_8(flight_data.roll, &obc_su_scripts.rx_buf[4]);
            cnv16_8(flight_data.pitch, &obc_su_scripts.rx_buf[6]);
            cnv16_8(flight_data.yaw, &obc_su_scripts.rx_buf[8]);
            cnv16_8(flight_data.roll_dot, &obc_su_scripts.rx_buf[10]);
            cnv16_8(flight_data.pitch_dot, &obc_su_scripts.rx_buf[12]);
            cnv16_8(flight_data.yaw_dot, &obc_su_scripts.rx_buf[14]);
            cnv16_8(flight_data.x_eci, &obc_su_scripts.rx_buf[16]);
            cnv16_8(flight_data.y_eci, &obc_su_scripts.rx_buf[18]);
            cnv16_8(flight_data.z_eci, &obc_su_scripts.rx_buf[20]);

            uint16_t size = SU_MAX_RSP_SIZE;
            mass_storage_storeLogs(SU_LOG, obc_su_scripts.rx_buf, &size);
        }
    }
    return SATR_OK;
}

uint8_t time_lala = 5;

void su_SCH() {

    if(obc_su_scripts.state == su_idle) {
        for(MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
            if(obc_su_scripts.scripts[i].invalid != true && obc_su_scripts.scripts[i].header.start_time >= time_lala && obc_su_scripts.scripts[i].header.start_time != 0) {
                
                obc_su_scripts.state = su_running;
                obc_su_scripts.active_script = i;
                
                mass_storage_su_load_api(i, obc_su_scripts.active_buf);
                obc_su_scripts.tt_pointer_curr = SU_TT_OFFSET;

                /*finds the next tt that needs to be executed, it iterates all the tt to find the correct one*/
                for(uint16_t b = SU_TT_OFFSET; b < SU_MAX_FILE_SIZE; b++) {

                    su_next_tt(obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr);

                    if(obc_su_scripts.tt_header.script_index == SU_SCR_TT_EOR) { break; }
                   // else if(obc_su_scripts.tt_header.time < time_now()) { su_next_tt(obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr); }
                    else { break; }
                    if(!C_ASSERT(obc_su_scripts.tt_pointer_curr < obc_su_scripts.scripts[i].header.script_len) == true) { break; }
                }
            }
        }
    }
    if(obc_su_scripts.state == su_running) {
      //  if(obc_su_scripts.tt_header.time >= time_now()) { 
            if(obc_su_scripts.tt_header.script_index == SU_SCR_TT_EOR) { obc_su_scripts.state = su_finished; }
            else {
              SAT_returnState res;
              su_tt_handler(obc_su_scripts.tt_header, obc_su_scripts.scripts, obc_su_scripts.active_script, &obc_su_scripts.script_pointer_curr);
              su_next_cmd(obc_su_scripts.active_buf, &obc_su_scripts.cmd_header, &obc_su_scripts.script_pointer_curr);
              
              for(uint16_t i = obc_su_scripts.script_pointer_curr; i < SU_MAX_FILE_SIZE; i++) {

                  res = su_cmd_handler(&obc_su_scripts.cmd_header);
                  if(res == SATR_EOT) { break; }
                  su_next_cmd(obc_su_scripts.active_buf, &obc_su_scripts.cmd_header, &obc_su_scripts.script_pointer_curr);
              }
              su_next_tt(obc_su_scripts.active_buf, &obc_su_scripts.tt_header, &obc_su_scripts.tt_pointer_curr); 
          
            }
     //   }
    }
 //   if(time_now() == midnight) {
        su_INIT();
        obc_su_scripts.state = su_idle;
 //   }
}

void su_INIT() {
    for(MS_sid i = SU_SCRIPT_1; i <= SU_SCRIPT_7; i++) {
        obc_su_scripts.scripts[(uint8_t)i-1].invalid = false;
        SAT_returnState res = mass_storage_su_load_api(i, obc_su_scripts.temp_buf);
        if(res == SATR_ERROR || res == SATR_CRC_ERROR) { obc_su_scripts.scripts[(uint8_t)i-1].invalid = true; break; }
        su_populate_header(&obc_su_scripts.scripts[(uint8_t)i-1].header, obc_su_scripts.temp_buf);
        su_populate_scriptPointers(&obc_su_scripts.scripts[(uint8_t)i-1], obc_su_scripts.temp_buf);       
    }
    obc_su_scripts.rx_cnt = SU_SCI_HEADER;
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

    HAL_sys_delay((cmd->dt_min * 60) + cmd->dt_sec);

    if(cmd->cmd_id == SU_OBC_SU_ON_CMD_ID)          { su_power_ctrl(P_ON); } 
    else if(cmd->cmd_id == SU_OBC_SU_OFF_CMD_ID)    { su_power_ctrl(P_OFF); } 
    else if(cmd->cmd_id == SU_OBC_EOT_CMD_ID)       { return SATR_EOT; } 
    else                                            { HAL_su_uart_tx(&cmd->pointer, cmd->len); }

    return SATR_OK;
}

SAT_returnState su_populate_header(struct script_hdr *hdr, uint8_t *buf) {


    if(!C_ASSERT(buf != NULL && hdr != NULL) == true)          { return SATR_ERROR; }

    union _cnv cnv;

    cnv.cnv8[0] = buf[0];
    cnv.cnv8[1] = buf[1];
    hdr->script_len = cnv.cnv16[0];

    cnv.cnv8[0] = buf[2];
    cnv.cnv8[1] = buf[3];
    cnv.cnv8[2] = buf[4];
    cnv.cnv8[3] = buf[5];
    hdr->start_time = cnv.cnv32;
    
    cnv.cnv8[0] = buf[6];
    cnv.cnv8[1] = buf[7];
    cnv.cnv8[2] = buf[8];
    cnv.cnv8[3] = buf[9];
    hdr->file_sn = cnv.cnv32;

    hdr->sw_ver = 0x1F & buf[10];
    hdr->su_id = 0x03 & (buf[10] >> 5); //need to check this
    hdr->script_type = 0x1F & buf[11];
    hdr->su_md = 0x03 & (buf[11] >> 5); //need to check this

    cnv.cnv8[0] = buf[hdr->script_len-2];
    cnv.cnv8[1] = buf[hdr->script_len-1];
    hdr->xsum = cnv.cnv16[0];

    return SATR_OK;
}  

SAT_returnState su_populate_scriptPointers(struct su_script *su_scr, uint8_t *buf) {

    uint16_t tt_temp = 0;

    tt_temp = SU_TT_OFFSET;

    struct script_times_table temp_tt_header;
    
    su_scr->header.scr_sequences = 0;
    
    for(uint16_t i = SU_TT_OFFSET; i < SU_MAX_FILE_SIZE; i++) {
        su_next_tt(buf, &temp_tt_header, &tt_temp);
        if(temp_tt_header.script_index == SU_SCR_TT_EOR) { break; }
        if(!C_ASSERT(tt_temp < su_scr->header.script_len) == true) { break; }
    }

    su_scr->script_pointer_start[0] = tt_temp;
    uint16_t script_temp = su_scr->script_pointer_start[0];

    for(uint8_t a = 1; a < SU_CMD_SEQ; a++) {

        if(script_temp == su_scr->header.script_len - 2) { break; }

        for(uint16_t b = 0; b < SU_MAX_FILE_SIZE; b++) {
            struct script_seq temp_cmd_header = {0};
            if(su_next_cmd(buf, &temp_cmd_header, &script_temp) == SATR_ERROR) { break; };
            if(temp_cmd_header.cmd_id == SU_OBC_EOT_CMD_ID && script_temp != su_scr->header.script_len - 2) { 
                su_scr->script_pointer_start[a] = script_temp;
                su_scr->header.scr_sequences++;
                break; 
            } else if(script_temp == su_scr->header.script_len - 2) { break; }
        }
        
    }
    if(!C_ASSERT(su_scr->header.scr_sequences < SU_CMD_SEQ) == true) { return SATR_ERROR; }

    return SATR_OK;
}

SAT_returnState su_next_tt(uint8_t *buf, struct script_times_table *tt, uint16_t *pointer) {

    if(!C_ASSERT(buf != NULL && tt != NULL && pointer != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->script_index != SU_SCR_TT_EOR) == true) { return SATR_EOT; }

    tt->sec = buf[(*pointer)++];
    tt->min = buf[(*pointer)++];
    tt->hours = buf[(*pointer)++];
    tt->script_index = buf[(*pointer)++];

    if(!C_ASSERT(tt->sec < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->min < 59) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->hours < 23) == true) { return SATR_ERROR; }
    if(!C_ASSERT(tt->script_index == SU_SCR_TT_S1 || \
                 tt->script_index == SU_SCR_TT_S2 || \
                 tt->script_index == SU_SCR_TT_S3 || \
                 tt->script_index == SU_SCR_TT_S4 || \
                 tt->script_index == SU_SCR_TT_S5 || \
                 tt->script_index == SU_SCR_TT_EOR) == true) { return SATR_ERROR; }

    return SATR_OK;
}

SAT_returnState su_next_cmd(uint8_t *buf, struct script_seq *cmd, uint16_t *pointer) {

    if(!C_ASSERT(buf != NULL && cmd != NULL && pointer != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(cmd->cmd_id != SU_OBC_EOT_CMD_ID) == true) { return SATR_EOT; }

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

SAT_returnState su_power_ctrl(FM_fun_id fid) {

    tc_tm_pkt *temp_pkt = 0;

    function_management_pctrl_crt_pkt_api(&temp_pkt, EPS_APP_ID, fid, SU_DEV_ID);
    if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

    route_pkt(temp_pkt);

    return SATR_OK;
}
