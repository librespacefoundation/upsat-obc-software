#ifndef __SERVICES_H
#define __SERVICES_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* TM TC services*/
#define ECSS_VER_NUMBER             0
#define ECSS_DATA_FIELD_HDR_FLG     1
// never used
//#define TC_TM_SER_TC_VER            1

#define ECSS_PUS_VER            1
#define ECSS_SEC_HDR_FIELD_FLG  0

/*sequence definitions*/
/*we only support TC_TM_SEQ_SPACKET*/
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

//needs to redifine
#define MAX_PKT_DATA    20
#define MAX_EX_PKT_DATA 512
#define TC_MAX_PKT_SIZE 515 //random

#define ECSS_HEADER_SIZE        6
#define ECSS_DATA_HEADER_SIZE   4
#define ECSS_CRC_SIZE           2

#define ECSS_DATA_OFFSET        ECSS_HEADER_SIZE + ECSS_DATA_HEADER_SIZE

#define MAX_APP_ID      20
#define MAX_SERVICES    20
#define MAX_SUBTYPES    26

#define TC 1
#define TM 0

typedef enum {  
    SATR_PKT_ILLEGAL_APPID     = 0,
    SATR_PKT_INV_LEN           = 1,
    SATR_PKT_INC_CRC           = 2,
    SATR_PKT_ILLEGAL_PKT_TP    = 3,
    SATR_PKT_ILLEGAL_PKT_STP   = 4,
    SATR_PKT_ILLEGAL_APP_DATA  = 5,
    SATR_OK                    = 6,
    SATR_ERROR                 = 7,
    SATR_EOT                   = 8,
    SATR_CRC_ERROR             = 9,
    SATR_PKT_ILLEGAL_ACK       = 10,
    SATR_ALREADY_SERVICING     = 11,
    SATR_MS_MAX_FILES          = 12,
    SATR_PKT_INIT              = 13,
    SATR_LAST                  = 14
}SAT_returnState;

/*services types*/
#define TC_VERIFICATION_SERVICE         1
#define TC_HOUSEKEEPING_SERVICE         3
#define TC_FUNCTION_MANAGEMENT_SERVICE  8
#define TC_SCHEDULING_SERVICE           11
#define TC_LARGE_DATA_SERVICE           13
#define TC_MASS_STORAGE_SERVICE         15
#define TC_TEST_SERVICE                 17

/*services subtypes*/
#define TM_VR_ACCEPTANCE_SUCCESS        1
#define TM_VR_ACCEPTANCE_FAILURE        2

#define TC_HK_REPORT_PARAMETERS         21
#define TM_HK_PARAMETERS_REPORT         23

#define TC_FM_PERFORM_FUNCTION          1

#define TC_SC_ENABLE_RELEASE            1
#define TC_SC_DISABLE_RELEASE           2
#define TC_SC_RESET_SCHEDULE            3
#define TC_SC_INSERT_TC                 4
#define TC_SC_DELETE_TC                 5
#define TC_SC_TIME_SHIFT_SPECIFIC       7
#define TC_SC_TIME_SHIFT_SELECTED_OTP   8
#define TC_SC_TIME_SHIFT_ALL            15

#define TM_LD_FIRST_DOWNLINK            1
#define TC_LD_FIRST_UPLINK              9
#define TM_LD_INT_DOWNLINK              2
#define TC_LD_INT_UPLINK                10
#define TM_LD_LAST_DOWNLINK             3
#define TC_LD_LAST_UPLINK               11
#define TC_LD_ACK_DOWNLINK              5
#define TM_LD_ACK_UPLINK                14
#define TC_LD_REPEAT_DOWNLINK           6
#define TM_LD_REPEAT_UPLINK             15
#define TM_LD_REPEATED_DOWNLINK         7
#define TC_LD_REPEATED_UPLINK           12
#define TM_LD_ABORT_SE_DOWNLINK         4
#define TC_LD_ABORT_SE_UPLINK           13
#define TC_LD_ABORT_RE_DOWNLINK         8
#define TM_LD_ABORT_RE_UPLINK           16
#define TM_LD_STANDALONE_DOWNLINK       17 /*custom*/
#define TC_LD_STANDALONE_UPLINK         18 /*custom*/

#define TC_MS_ENABLE                    1
#define TC_MS_DISABLE                   2
#define TC_MS_DOWNLINK                  9
#define TC_MS_DELETE                    11
#define TC_MS_REPORT                    12
#define TM_MS_CATALOGUE_REPORT          13

#define TC_CT_PERFORM_TEST              1
#define TM_CT_REPORT_TEST               2

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
    HEALTH_REP      = 1,
    EX_HEALTH_REP   = 2,
    EVENTS_REP      = 3,
    WOD_REP         = 4,
    LAST_STRUCT_ID  = 5
}HK_struct_id;

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

/*Mass storage ids*/
typedef enum {  
    SU_SCRIPT_1     = 1,
    SU_SCRIPT_2     = 2,
    SU_SCRIPT_3     = 3,
    SU_SCRIPT_4     = 4,
    SU_SCRIPT_5     = 5,
    SU_SCRIPT_6     = 6,
    SU_SCRIPT_7     = 7,
    SU_LOG          = 8,
    EVENT_LOG       = 9,
    FOTOS           = 10,
    TMP_SU_SCRIPT_1 = 11,
    TMP_SU_SCRIPT_2 = 12,
    TMP_SU_SCRIPT_3 = 13,
    TMP_SU_SCRIPT_4 = 14,
    TMP_SU_SCRIPT_5 = 15,
    TMP_SU_SCRIPT_6 = 16,
    TMP_SU_SCRIPT_7 = 17,
    LAST_SID        = 18
}MS_sid;

typedef enum {  
    ALL         = 0,
    TO          = 1,
    BETWEEN     = 2,
    SPECIFIC    = 3,
    LAST_PART   = 4,
    NO_MODE     = 5,
    LAST_MODE   = 6
}MS_mode;

#define C_ASSERT(e)    ((e) ? (true) : (tst_debugging( __FILE__, __LINE__, #e))) 

union _cnv {
    uint32_t cnv32;
    uint16_t cnv16[2];
    uint8_t cnv8[4];
};
extern void HAL_eps_uart_tx(uint8_t *buf, uint16_t size);
typedef struct {
    /* packet id */
    //uint8_t ver; /* 3 bits, should be equal to 0 */

    //uint8_t data_field_hdr; /* 1 bit, data_field_hdr exists in data = 1 */
    TC_TM_app_id app_id; /* TM: app id = 0 for time packets, = 0xff for idle packets. should be 11 bits only 8 are used though */
    uint8_t type; /* 1 bit, tm = 0, tc = 1 */

    /* packet sequence control */
    uint8_t seq_flags; /* 3 bits, definition in TC_SEQ_xPACKET */
    uint16_t seq_count; /* 14 bits, packet counter, should be unique for each app id */

    uint16_t len; /* 16 bits, C = (Number of octets in packet data field) - 1, on struct is the size of data without the headers. on array is with the headers */

    uint8_t ack; /* 4 bits, definition in TC_ACK_xxxx 0 if its a TM */
    uint8_t ser_type; /* 8 bit, service type */
    uint8_t ser_subtype; /* 8 bit, service subtype */

    /*optional*/
    //uint8_t pckt_sub_cnt; /* 8 bits*/
    TC_TM_app_id dest_id;   /*on TC is the source id, on TM its the destination id*/

    uint8_t *data; /* variable data, this should be fixed array, normal or extended */

    /*this is not part of the header. it is used from the software and the verification service,
     *when the packet wants ack. 
     *the type is SAT_returnState and it either stores R_OK or has the error code (failure reason).
     *it is initiazed as R_ERROR and the service should be responsible to make it R_OK or put the coresponding error.     
     */
    SAT_returnState verification_state; 
/*  uint8_t padding;  x bits, padding for word alligment */

//  uint16_t crc; /* CRC or checksum, mission specific*/
}tc_tm_pkt;

/*Lookup table that returns if a service with its subtype with TC or TM is supported and valid*/
extern const uint8_t services_verification_TC_TM[MAX_SERVICES][MAX_SUBTYPES][2];

extern const uint8_t services_verification_OBC_TC[MAX_SERVICES][MAX_SUBTYPES];

//ToDo
//  define in unpack the MIN_PKT_SIZE and MAX_PKT_SIZE
//  need to check pkt len for not overruning to checksum
//  sort definitions relating to file system and packet sizes etc.
//  update verification lookup table
//  add verification steps in each service.
//  assert for 0 in modes, ids when applicable.
//  verify HK_struct_id modes
//  check that cnv functions are used correctly
//  function management set time.
//  finalize TC_MAX_PKT_SIZE
//  add reset counter, reset source finder.
//  add event log book function
//  test assertion definition for stm
//  finish assertions
//  add assertions in each service for its subtype
//  architecture overview
//  add definitions for packet len calculations

//finished
//  CRC in 8bits instead of 16 but use it anyway. the high byte should be 0.
//  there is no support for verification for obc, do we need that?
//  SAT_returnState renaming to UPS_OK?
//  what to do with verification service, after route or after its service.
//  should we move all utilities functions, like pack, route etc in one big function file?
//  migrate verification on pkt status bit: add status byte in tc_tm pkt, add support for each service, make sure route works
//  when to free the packets.
//  definitions of subtypes.
//  modify route & verification.
//  add function management service.
//  add serial.
//  use cnv functions.
//  use packet len instead of individual service pack, for pack.
//  rename tc_tm.h, use it as a header only, move .c in service_utilities.
//  add seq count in pack and global memory.
//  use pkt->len for data?
//  add pack functions in each service.

//stub
uint32_t time_now();

uint8_t tst_debugging(char *f, int l, char *e);

#endif
