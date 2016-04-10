#ifndef SERVICE_UTILITIES_H
#define SERVICE_UTILITIES_H

#include <stdint.h>
#include "services.h"

//temp
#define TEST_ARRAY 1024
#define MIN_PKT_SIZE 5
#define MAX_PKT_SIZE 1024

#define EV_MAX_BUFFER 1024

#define WOD_MAX_BUFFER 64

/*restriction for 8 char filename, for conversion from num to file name*/
#define MAX_FILE_NUM 0x5F5E0FF

extern struct _obc_data obc_data;
extern struct _sat_status sat_status;

extern tc_tm_pkt * get_pkt();
extern SAT_returnState free_pkt(tc_tm_pkt *pkt);
extern uint32_t time_now();

extern SAT_returnState verification_app(tc_tm_pkt *pkt); 
extern SAT_returnState hk_app(tc_tm_pkt *pkt);
extern SAT_returnState function_management_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size);
extern SAT_returnState large_data_app(tc_tm_pkt *pkt);
extern SAT_returnState test_app(tc_tm_pkt *pkt);

extern SAT_returnState unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size);
extern SAT_returnState pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size);

extern SAT_returnState HLDLC_deframe(uint8_t *buf_in, uint8_t *buf_out, uint16_t *size);
extern SAT_returnState HLDLC_frame(uint8_t *buf_in, uint8_t *buf_out, uint16_t *size);

extern SAT_returnState HAL_eps_uart_rx();
extern void HAL_eps_uart_tx(uint8_t *buf, uint16_t size);
extern uint32_t * HAL_obc_BKPSRAM_BASE();

//ToDo
//  seq counter, hoq it works?
//  need to make an obc struct with all the data.
//  Add if condition for normal packet or extended

//Finished
//  fix in unpack the pkt len - 4

SAT_returnState checkSum(const uint8_t *data, const uint16_t size, uint8_t *res_crc);

SAT_returnState obc_data_INIT();

SAT_returnState route_pkt(tc_tm_pkt *pkt);

SAT_returnState import_eps_pkt();

SAT_returnState export_eps_pkt(tc_tm_pkt *pkt);

SAT_returnState unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size);

SAT_returnState pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size);

SAT_returnState crt_pkt(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, TC_TM_app_id dest_id);


void bkup_sram_INIT();

uint32_t get_new_fileId();

SAT_returnState update_boot_counter();

SAT_returnState get_boot_counter(uint32_t *cnt);


SAT_returnState event_log(uint8_t *buf, const uint16_t size);

SAT_returnState event_log_load(uint8_t *buf, const uint16_t pointer, const uint16_t size);

SAT_returnState event_log_IDLE();


void cnv32_8(const uint32_t from, uint8_t *to);

void cnv16_8(const uint16_t from, uint8_t *to);

void cnv8_32(uint8_t *from, uint32_t *to);

void cnv8_16(uint8_t *from, uint16_t *to);

#endif