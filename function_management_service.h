#ifndef __FUNCTION_MANAGEMENT_SERVICE_H
#define __FUNCTION_MANAGEMENT_SERVICE_H

#include <stdint.h>
#include "services.h"

extern uint32_t time_now();

//ToDo
//  maybe make dev id = app id

OBC_returnStateTypedef function_management_app(tc_tm_pkt *pkt);

#endif