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
OBC             = 1,
EPS             = 2,
ADCS            = 3,
COMMS           = 4,
IAC             = 5,
SU              = 6,
GPS             = 7,
OBC_SD          = 8,
ADCS_SD         = 9,
LAST_DEV_ID     = 10
}FM_dev_id;

//ToDo
//  maybe make dev id = app id

OBC_returnStateTypedef function_management_app(tc_tm_pkt *pkt);

#endif