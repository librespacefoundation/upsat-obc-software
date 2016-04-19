#ifndef __EVENT_REPORTING_SERVICE_H
#define __EVENT_REPORTING_SERVICE_H

#include <stdint.h>
#include "services.h"

extern SAT_returnState checkSum(const uint8_t *data, const uint16_t size, uint8_t *res_crc);

//ToDo

SAT_returnState event_crt_pkt_api(uint8_t *buf, char *f, int fi, int l, char *e, uint16_t *size, SAT_returnState mode);

#endif