#ifndef SERVICE_UTILITIES_H
#define SERVICE_UTILITIES_H

#include <stdint.h>
#include "tc_tm.h"

uint8_t checkSum(const uint8_t *data, uint16_t size);

OBC_returnStateTypedef unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size);

OBC_returnStateTypedef pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size);

OBC_returnStateTypedef crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id);

void cnv32_8(const uint32_t from, uint8_t *to);

void cnv16_8(const uint16_t from, uint8_t *to);

void cnv8_32(uint8_t *from, uint32_t *to);

void cnv8_16(uint8_t *from, uint16_t *to);

#endif