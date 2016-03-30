#ifndef SU_MNLP_H
#define SU_MNLP_H

#include <stdint.h>

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
#define SU_SCR_TT_EOR 0x55

/*
Section 13.10

when 0x00 is undefined
*/
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

#define SU_TT_OFFSET    12

struct OBC_data {
    uint32_t time_epoch;
    int16_t  roll;
    int16_t  pitch;
    int16_t  yaw;
    int16_t  roll_dot;
    int16_t  pitch_dot;
    int16_t  yaw_dot;
    uint16_t x_eci;
    uint16_t y_eci;
    uint16_t z_eci;
};

struct script_handler
{
    uint16_t su_timeout
    uint8_t state;
    struct  su_script scripts[SU_MAX_SCRIPTS];
    
    uint8_t temp_buf[MS_MAX_SU_FILE_SIZE];
    uint8_t active_buf[MS_MAX_SU_FILE_SIZE];  
    MS_sid active_script;

    struct script_times_table tt_header;
    struct script_seq cmd_header;

    uint16_t tt_pointer_curr;
    uint16_t script_pointer_curr;
};

struct su_script {
    struct script_hdr header;
    uint16_t script_pointer_start[SU_CMD_SEQ];
};

/* 
section 13.6

time format?
ID_SWver: b4-b0 sw ver,b6-b5 su id 
MD_type:  b4-b0 script type, b6-b5 su model
*/
struct script_hdr 
{
    uint16_t script_len;
    uint32_t start_time;
    uint32_t file_sn;
    uint8_t  sw_ver;
    uint8_t  su_id;
    uint8_t  script_type;
    uint8_t  su_md;
};

/* 
REQ: MNLP-027
*/
struct script_times_table
{
    uint8_t sec;
    uint8_t min;
    uint8_t hours;
    uint8_t script_index;
};

/* 
REQ: MNLP-027
*/
struct script_seq
{
    uint8_t dt_sec;
    uint8_t dt_min;
    uint8_t cmd_id;
    uint8_t len;
    uint8_t seq_cnt;
    uint8_t parameters[255];
};

/*


*/
struct science_data {
    struct science_hdr header;
    struct response_pckt res_pck;
}; 

/*
REQ: MNLP-022

*/
struct science_hdr 
{
    uint32_t time_epoch;
    int16_t  roll;
    int16_t  pitch;
    int16_t  yaw;
    int16_t  roll_dot;
    int16_t  pitch_dot;
    int16_t  yaw_dot;
    uint16_t x_eci;
    uint16_t y_eci;
    uint16_t z_eci;
};

/*
REQ: MNLP-031

*/
struct response_pckt 
{
    uint8_t rsp_id;
    uint8_t seq_cnt;
    uint8_t data[SU_RSP_PCKT_DATA_SIZE];
};

extern struct script_handler obc_su_scripts;

#endif