#ifndef SU_MNLP_H
#define SU_MNLP_H

#include <stdint.h>
#include "services.h"

#define SU_MAX_SCRIPTS_POPU 7
#define SU_MAX_RSP_SIZE 174
#define SU_MAX_FILE_SIZE 2048
#define SU_CMD_SEQ 5
#define SU_SCI_HEADER 22
#define SU_EOT_CMD_HEADER_SIZE 5
#define SU_TT_HEADER_SIZE 4

/*  
REQ: MNLP-013
*/
#define SU_TIMEOUT 400 

/* 
section 13.6
*/
#define SU_SCR_HDR_RES   0
#define SU_SCR_HDR_INMS  1
#define SU_SCR_HDR_MNLP  2
#define SU_SCR_HDR_FIPEX 3

/*
REQ: MNLP-027
*/
#define SU_SCR_TT_S1  0x41
#define SU_SCR_TT_S2  0x42
#define SU_SCR_TT_S3  0x43
#define SU_SCR_TT_S4  0x44
#define SU_SCR_TT_S5  0x45
#define SU_SCR_TT_SNONE = 0x00
#define SU_SCR_TT_EOT 0x55

/*
Section 13.10

when 0x00 is undefined
*/

/*SU command IDs*/
#define SU_OBC_SU_ON_CMD_ID     0xF1
#define SU_OBC_SU_OFF_CMD_ID    0xF2
#define SU_RESET_CMD_ID         0x02
#define SU_LDP_CMD_ID           0x05
#define SU_HC_CMD_ID            0x06
#define SU_CAL_CMD_ID           0x07
#define SU_SCI_CMD_ID           0x08
#define SU_HK_CMD_ID            0x09
#define SU_STM_CMD_ID           0x0A
#define SU_DUMP_CMD_ID          0x0B
#define SU_BIAS_ON_CMD_ID       0x53
#define SU_BIAS_OFF_CMD_ID      0xC9
#define SU_MTEE_ON_CMD_ID       0x35
#define SU_MTEE_OFF_CMD_ID      0x9C
#define SU_ERR_CMD_ID           0x00
#define SU_OBC_SU_ERR_CMD_ID    0x00
#define SU_OBC_EOT_CMD_ID       0xFE

/*SU responses IDs*/
#define OBC_SU_ON_RSP_ID    0x00
#define OBC_SU_OFF_RSP_ID   0x00
#define SU_RESET_RSP_ID     0x00
#define SU_LDP_RSP_ID       0x05
#define SU_HC_RSP_ID        0x06
#define SU_CAL_RSP_ID       0x07
#define SU_SCI_RSP_ID       0x08
#define SU_HK_RSP_ID        0x09
#define SU_STM_RSP_ID       0x0A
#define SU_DUMP_RSP_ID      0x0B
#define SU_BIAS_ON_RSP_ID   0x00
#define SU_BIAS_OFF_RSP_ID  0x00
#define SU_MTEE_ON_RSP_ID   0x00
#define SU_MTEE_OFF_RSP_ID  0x00
#define SU_ERR_RSP_ID       0xBB
#define OBC_SU_ERR_RSP_ID   0xFA
#define OBC_EOT_RSP_ID      0x00

/*
REQ: MNLP-031
*/
#define SU_RSP_PCKT_DATA_SIZE  172
#define SU_RSP_PCKT_SIZE       174

/* the 13th byte is the first byte 
 * of a time table script record.
 */
#define SU_TT_OFFSET    12

/*
 *This header is to be attached in every response from the m-nlp scientific instrument.
 *needs to be populated in every response in order to have up to date info.
 */
typedef struct{
    
    /*qb50_epoch, seconds since Year 2000*/
    uint32_t time_epoch;
    /*-180to+180 deg*/
    int16_t  roll;
    /*-180to+180 deg*/
    int16_t  pitch;
    /*-180to+180 deg*/
    int16_t  yaw;
    /*deg/sec*/
    int16_t  roll_dot;
    /*deg/sec*/
    int16_t  pitch_dot;
    /*deg/sec*/
    int16_t  yaw_dot;
    /*km*/
    uint16_t x_eci;
    /*km*/
    uint16_t y_eci;
    /*km*/
    uint16_t z_eci;
    
}mnlp_response_science_header;

/*
fixed size: 12 bytes
section 13.6
time format: UTC
ID_SWver: b4-b0 sw ver,b6-b5 su id 
MD_type:  b4-b0 script type, b6-b5 su model
*/
typedef struct 
{
    /*complete script length, 2 bytes*/
    uint16_t script_len;
    /*UTC time at which script execution begins, 4 bytes*/
    uint32_t start_time;
    /*file serial number, 4 bytes*/
    uint32_t file_sn;
    /*SW_VER-> 5bits, SU_ID-> 2 bits, b7->1 bit "0", 1 byte*/
    uint8_t  sw_ver;
    uint8_t  su_id;
    /*usage of script, script type-> 5 bits, su_model-> 2 bits, b7->1 bit "0" */
    uint8_t  script_type;
    /*su model*/
    uint8_t  su_md;
    /*extral chexksum?*/
    uint16_t xsum;
    /*number of script sequences in the times table*/
    uint8_t  script_sequences_popu;
}science_unit_script_header;

/* 
REQ: MNLP-027
*/
typedef struct
{
    /*range: 00-59*/
    uint8_t sec;
    /*range: 00-59*/
    uint8_t min;
    /*range: 00-23*/
    uint8_t hours;
    /*range: 0x41->S1, 0x42->S2, 0x43->S3, 0x44->S4, 0x45->S5, 0x55->EndOfTable*/
    uint8_t script_index;    
}science_unit_script_time_table;

/* 
REQ: MNLP-027
*/
typedef struct
{
    /*deltaTIME seconds, range: 00-59*/
    uint8_t dt_sec;
    /*deltaTIME minutes, range: 00-59*/
    uint8_t dt_min;
    /*CMD_ID, command id*/
    uint8_t cmd_id;
    /*LEN, length in BYTES of parameter field, range: 0 - 255*/
    uint8_t len;
    /*SEQ_CNT, command counter, range: 0 - 255*/
    uint8_t seq_cnt;
    /**/
    uint8_t parameters[255];
    /**/
    uint8_t pointer;
}science_unit_script_sequence;

//typedef struct{
//    su_script_header header;
//    uint16_t script_pointer_start[SU_CMD_SEQ];
//    uint8_t invalid;
//}su_script ;

/*represents a science unit script, with its header, time table and command sequences*/
typedef struct
{
    science_unit_script_header scr_header;
    science_unit_script_time_table tt_header;
    science_unit_script_sequence seq_header;
    
    /*where the data will be kept after loading*/
    uint8_t file_load_buf[SU_MAX_FILE_SIZE];
    
    /*a script is valid if it have passed the checksums checks */
    uint8_t valid;
    
    /*a script is active if it is the now-running-active script*/
    uint8_t active;
    
    uint16_t tt_pointer_curr;
    
    uint32_t timeout;
    uint16_t rx_cnt;
    uint8_t rx_buf[SU_MAX_RSP_SIZE];    
    
    uint8_t active_buf[SU_MAX_FILE_SIZE];  
    
    uint16_t script_pointer_curr;
    
}science_unit_script_inst;

extern science_unit_script_inst su_scripts[SU_MAX_SCRIPTS_POPU];
extern mnlp_response_science_header flight_data;

extern SAT_returnState function_management_pctrl_crt_pkt_api(tc_tm_pkt **pkt, TC_TM_app_id dest_id, FM_fun_id fun_id, FM_dev_id did);
extern SAT_returnState route_pkt(tc_tm_pkt *pkt);
  
extern SAT_returnState HAL_su_uart_rx(uint8_t *c);
extern void HAL_su_uart_tx(uint8_t *buf, uint16_t size);
extern void HAL_sys_delay(uint32_t sec);

extern SAT_returnState mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size);

extern SAT_returnState mass_storage_su_load_api(MS_sid sid, uint8_t *buf);

//ToDo
//  add check for su status off
//  add calendar

void su_INIT();

/*
 * Loads the scipts from permanent storage.
 */
void su_load_scripts();

/*this is to be called from a freeRTOS task, continually*/
void su_SCH();

/* 
 * traverses the script sequences to find the one requested by
 * *ss_to_go parameter, the resulting offset is stored on 
 * *script_sequence_pointer.
 */
SAT_returnState su_goto_script_seq( uint16_t *script_sequence_pointer, uint8_t *ss_to_go);
//void su_timeout_handler(uint8_t error);

SAT_returnState su_incoming_rx();

//SAT_returnState su_tt_handler(science_unit_script_times_table tt, su_script *scripts, MS_sid active_script, uint16_t *start);

SAT_returnState su_cmd_handler( science_unit_script_sequence *cmd);
    
/* 
 * TITLE: QB50 m-NLP Science Unit Interface Control Document
 * Document Number: QB50-UiO-ID-0001 M-NLP ICD Issue 6.2
 * Date 22.10.2015, page 51.
 * Header fixed size is 12 bytes.
 */
SAT_returnState su_populate_header( science_unit_script_header *su_script_hdr, uint8_t *buf);

//SAT_returnState su_populate_scriptPointers( su_script *su_scr, uint8_t *buf);

SAT_returnState polulate_next_time_table(uint8_t *buf, science_unit_script_time_table *tt, uint16_t *tt_pointer);

SAT_returnState su_next_cmd(uint8_t *buf,  science_unit_script_sequence *cmd, uint16_t *pointer);

SAT_returnState su_power_ctrl(FM_fun_id fid);

#endif
