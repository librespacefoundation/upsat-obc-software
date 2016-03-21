#ifndef __TC_TM_H
#define __TC_TM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* TM TC services*/
#define ECSS_VER_NUMBER             0
#define ECSS_DATA_FIELD_HDR_FLG     1
#define TC_TM_SER_TC_VER            1

#define ECSS_PUS_VER            1
#define ECSS_SEC_HDR_FIELD_FLG  0
/* Minimal */
#define TC_TM_SER_TC_VER_ACC_SUCC           1
#define TC_TM_SER_TC_VER_ACC_FAIL           2
/* Additional */
#define TC_TM_SER_TC_VER_EXEC_START_SUCC    3
#define TC_TM_SER_TC_VER_EXEC_START_FAIL    4
#define TC_TM_SER_TC_VER_EXEC_PROG_SUCC     5
#define TC_TM_SER_TC_VER_EXEC_PROG_FAIL     6
#define TC_TM_SER_TC_VER_EXEC_COMP_SUCC     7
#define TC_TM_SER_TC_VER_EXEC_COMP_FAIL     8

#define TC_TM_SER_DIST      2
#define TC_TM_SER_HK        3

/* Minimal */
#define TC_TM_SER_HK_PAR    25
/* Additional */

#define TC_TM_SER_STAT      4
#define TC_TM_SER_EVENT     5
#define TC_TM_SER_MEM       6
#define TC_TM_SER_NC1       7
#define TC_TM_SER_FMAG      8
#define TC_TM_SER_TMAG      9
#define TC_TM_SER_NC2       10

#define TC_TM_SER_SCH       11
#define TC_TM_SER_MON       12
#define TC_TM_SER_LDATA     13
#define TC_TM_SER_PCKT_FWD  14
#define TC_TM_SER_STORAGE   15
#define TC_TM_SER_NC3       16
#define TC_TM_SER_TEST      17
#define TC_TM_SER_PROC      18
#define TC_TM_SER_EV_ACT    19

/*sequence definitions*/
#define TC_TM_SEQ_FPACKET 0x01
#define TC_TM_SEQ_CPACKET 0x00
#define TC_TM_SEQ_LPACKET 0x02
#define TC_TM_SEQ_SPACKET 0x03

/*services ack req*/
/*should confirm endianess*/
#define TC_ACK_NO           0x00
#define TC_ACK_ACC          0x01
#define TC_ACK_EXE_START    0x02
#define TC_ACK_EXE_PROG     0x04
#define TC_ACK_EXE_COMP     0x08
#define TC_ACK_ALL          0x0F

#define TC_TM_SER_LDATA_MAX_DATA_PKT    512

#define VER_NUMBER
#define TC_TM_TYPE
#define DATA_FIELD_HDR_FLG
#define APP_ID
#define SEQ_FLG

#define MAX_PKT_DATA    20
#define MAX_EX_PKT_DATA 512

#define ECSS_DATA_HEADER_SIZE 4 //need to check

#define MAX_APP_ID      20
#define MAX_SERVICES    20
#define MAX_SUBTYPES    26

#define TC_MAX_PKT_SIZE 515 //random

#define TC 1
#define TM 0

typedef enum {  
R_PKT_ILLEGAL_APPID     = 0,
R_PKT_INV_LEN           = 1,
R_PKT_INC_CRC           = 2,
R_PKT_ILLEGAL_PKT_TP    = 3,
R_PKT_ILLEGAL_PKT_STP   = 4,
R_PKT_ILLEGAL_APP_DATA  = 5,
R_OK                    = 6,
R_ERROR                 = 7,
R_EOT                   = 8,
R_CRC_ERROR             = 9,
R_PKT_ILLEGAL_ACK       = 10,
R_ALREADY_SERVICING     = 11,
R_LAST                  = 12
}OBC_returnStateTypedef;

#define TC_VERIFICATION_SERVICE         1
#define TC_HOUSEKEEPING_SERVICE         3
#define TC_FUNCTION_MANAGEMENT_SERVICE  8
#define TC_LARGE_DATA_SERVICE           13
#define TC_MASS_STORAGE_SERVICE         15
#define TC_TEST_SERVICE                 17

#define TC_VR_ACCEPTANCE_SUCCESS        1
#define TC_VR_ACCEPTANCE_FAILURE        2

#define TC_HK_REPORT_PARAMETERS         21
#define TC_HK_PARAMETERS_REPORT         23

#define TC_FM_PERFORM_FUNCTION          1

#define TC_LD_FIRST_DOWNLINK            1
#define TC_LD_FIRST_UPLINK              9
#define TC_LD_INT_DOWNLINK              2
#define TC_LD_INT_UPLINK                10
#define TC_LD_LAST_DOWNLINK             3
#define TC_LD_LAST_UPLINK               11
#define TC_LD_ACK_DOWNLINK              5
#define TC_LD_ACK_UPLINK                14
#define TC_LD_REPEAT_DOWNLINK           6
#define TC_LD_REPEAT_UPLINK             15
#define TC_LD_REPEATED_DOWNLINK         7
#define TC_LD_REPEATED_UPLINK           12
#define TC_LD_ABORT_SE_DOWNLINK         4
#define TC_LD_ABORT_SE_UPLINK           13
#define TC_LD_ABORT_RE_DOWNLINK         8
#define TC_LD_ABORT_RE_UPLINK           16
#define TC_LD_STANDALONE_DOWNLINK       17
#define TC_LD_STANDALONE_UPLINK         18

#define TC_MS_DOWNLINK                  9
#define TC_MS_DELETE                    11
#define TC_MS_REPORT                    12

#define TC_CT_PERFORM_TEST              1
#define TC_CT_REPORT_TEST               2

/*memory pool packet modes*/
#define NORMAL      1
#define EXTENDED    2

typedef enum {  
OBC_APP_ID      = 1,
EPS_APP_ID      = 2,
ADCS_APP_ID     = 3,
COMMS_APP_ID    = 4,
IAC_APP_ID      = 5,
GND_APP_ID      = 6,
LAST_APP_ID     = 7
}TC_TM_app_id;

typedef enum {
P_OFF       = 0,
P_ON        = 1,
P_RESET     = 2,
SET_TIME    = 3,
LAST_FUN_ID = 4
}FM_fun_id;

typedef enum {
OBC_DEV_ID      = 1,
EPS_DEV_ID      = 2,
ADCS_DEV_ID     = 3,
COMMS_DEV_ID    = 4,
IAC_DEV_ID      = 5,
SU_DEV_ID       = 6,
GPS_DEV_ID      = 7,
OBC_SD_DEV_ID   = 8,
ADCS_SD_DEV_ID  = 9,
LAST_DEV_ID     = 10
}FM_dev_id;

typedef enum {  
SU_SCRIPT_1         = 1,
SU_SCRIPT_2         = 2,
SU_SCRIPT_3         = 3,
SU_SCRIPT_4         = 4,
SU_SCRIPT_5         = 5,
SU_SCRIPT_6         = 6,
SU_SCRIPT_7         = 7,
SU_LOG              = 8,
EVENT_LOG           = 9,
FOTOS               = 10,
TMP_SU_SCRIPT_1     = 11,
TMP_SU_SCRIPT_2     = 12,
TMP_SU_SCRIPT_3     = 13,
TMP_SU_SCRIPT_4     = 14,
TMP_SU_SCRIPT_5     = 15,
TMP_SU_SCRIPT_6     = 16,
TMP_SU_SCRIPT_7     = 17,
LAST_SID            = 18
}MS_sid;

typedef enum {  
ALL         = 0,
TO          = 1,
BETWEEN     = 2,
SPECIFIC    = 3,
LAST_PART   = 4,
LAST_MODE   = 5
}MS_mode;

#define C_ASSERT(e)    ((e) ? (true) : tst_debugging("%s,%d: assertion '%s' failed\n", __FILE__, __LINE__, #e), false) 

union _cnv {
    uint32_t cnv32;
    uint16_t cnv16[2];
    uint8_t cnv8[4];
};

typedef struct {
    /* packet id */
    //uint8_t ver; /* 3 bits, should be equal to 0 */

    //uint8_t data_field_hdr; /* 1 bit, data_field_hdr exists in data = 1 */
    uint8_t app_id; /* TM: app id = 0 for time packets, = 0xff for idle packets. should be 11 bits only 8 are used though */
    uint8_t type; /* 1 bit, tm = 0, tc = 1 */

    /* packet sequence control */
    uint8_t seq_flags; /* 3 bits, definition in TC_SEQ_xPACKET */
    uint16_t seq_count; /* 14 bits, packet counter, should be unique for each app id */

    uint16_t len; /* 16 bits, C = (Number of octets in packet data field) - 1 */

    uint8_t ack; /* 4 bits, definition in TC_ACK_xxxx 0 if its a TM */
    uint8_t ser_type; /* 8 bit, service type */
    uint8_t ser_subtype; /* 8 bit, service subtype */

    /*optional*/
    //uint8_t pckt_sub_cnt; /* 8 bits*/
    uint16_t dest_id;

    uint8_t *data; /* variable data, this should be fixed array */
/*  uint8_t padding;  x bits, padding for word alligment */

//  uint16_t crc; /* CRC or checksum, mission specific*/
}tc_tm_pkt;

/*Lookup table that returns if a service with its subtype with TC or TM is supported and valid*/
const uint8_t services_verification_TC_TM[MAX_SERVICES][MAX_SUBTYPES][2];

const uint8_t services_verification_OBC_TC[MAX_SERVICES][MAX_SUBTYPES];

//ToDo
//  check that cnv functions are used correctly
//  function management set time.
//  finalize TC_MAX_PKT_SIZE
//  what to do with verification service, after route or after its service.
//  OBC_returnStateTypedef renaming to UPS_OK?
//  add reset counter, reset source finder.
//  add event log book function
//  test assertion definition for stm
//  finish assertions
//  add assertions in each service for its subtype
//  architecture overview
//  rename tc_tm.h, use it as a header only, move .c in service_utilities.
//  add definitions for packet len calculations
//  use packet len instead of individual service pack, for pack.
//  when to free the packets.
//  definitions of subtypes.
//  modify route & verification.
//  add function management service.
//  add serial.
//  use cnv functions.

//finished
//  use pkt->len for data?
//  add pack functions in each service.

//stub
uint32_t time_now();

#endif
