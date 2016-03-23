#ifndef SERVICE_UTILITIES_H
#define SERVICE_UTILITIES_H

#include <stdint.h>
#include "services.h"
#include "pkt_pool.h"
#include "obc_hal.h"

//temp
#define TEST_ARRAY 1024

extern void *get_pkt(uint8_t mode);
extern SAT_returnState free_pkt(tc_tm_pkt *pkt);
extern uint32_t time_now();

extern SAT_returnState verification_app(tc_tm_pkt *pkt, SAT_returnState res); 
extern SAT_returnState hk_app(tc_tm_pkt *pkt);
extern SAT_returnState function_management_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_app(tc_tm_pkt *pkt);
extern SAT_returnState large_data_app(tc_tm_pkt *pkt);
extern SAT_returnState test_app(tc_tm_pkt *pkt);

extern SAT_returnState unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size);
extern SAT_returnState pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size);

extern SAT_returnState HLDLC_deframe(uint8_t *buf, uint16_t *cnt, const uint8_t c);
extern SAT_returnState HLDLC_frame(uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);

extern void HAL_eps_uart_tx(uint8_t *buf, uint16_t size);

//ToDo
//	fix in unpack the pkt len - 4
//  Add if condition for normal packet or extended

uint8_t checkSum(const uint8_t *data, uint16_t size);

SAT_returnState route_pkt(tc_tm_pkt *pkt);

SAT_returnState import_eps_pkt();

SAT_returnState export_eps_pkt(tc_tm_pkt *pkt);

SAT_returnState unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size);

SAT_returnState pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size);

SAT_returnState crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id);

void cnv32_8(const uint32_t from, uint8_t *to);

void cnv16_8(const uint16_t from, uint8_t *to);

void cnv8_32(uint8_t *from, uint32_t *to);

void cnv8_16(uint8_t *from, uint16_t *to);

#endif