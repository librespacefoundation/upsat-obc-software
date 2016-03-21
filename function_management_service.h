#ifndef FUNCTION_MANAGEMENT_H
#define FUNCTION_MANAGEMENT_H

#include <stdint.h>

#include "tc_tm.h"

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

//ToDo
//  maybe make dev id = app id

OBC_returnStateTypedef function_management_app(tc_tm_pkt *pkt);

#endif