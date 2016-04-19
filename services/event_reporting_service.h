#ifndef __EVENT_REPORTING_SERVICE_H
#define __EVENT_REPORTING_SERVICE_H

#include <stdint.h>
#include "services.h"


//ToDo

SAT_returnState event_crt_pkt_api(uint8_t *buf, char *f, int fi, int l, char *e, uint16_t *size, SAT_returnState mode);

#endif