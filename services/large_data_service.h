#ifndef LARGE_DATA_SERVICE_H
#define LARGE_DATA_SERVICE_H

#include <stdint.h>
#include "services.h"
#include "mass_storage_service.h"

#define LD_PKT_DATA             512
#define LD_PKT_DATA_HDR_SIZE    3

#define LD_TX_TYPE_REPORT       1
#define LD_TX_TYPE_DOWNLINK     2

typedef enum {
LD_STATE_FREE           = 1,
LD_STATE_RECEIVING      = 2,
LD_STATE_TRANSMITING    = 3,
LD_STATE_REPORT         = 4,
LD_STATE_DOWNLINK       = 5,
LAST_STATE              = 6
}LD_states;

struct _ld_status {
    LD_states state;        /*service state machine, state variable*/
    MS_sid sid;            /*sid */
    TC_TM_app_id app_id;    /*destination app id*/
    uint8_t ld_num;         /**/
    uint32_t timeout;       /**/
    uint32_t fcurr;         /**/
    uint32_t fnext;         /**/
    uint8_t txType;         /**/
    uint8_t started;        /**/
    uint8_t lpacket_flag;   /**/
    MS_mode mode;           /**/
    uint32_t from;          /**/
    uint32_t to;            /**/
};

extern struct _ld_status LD_status;

extern tc_tm_pkt * get_pkt();
extern uint32_t time_now();

extern SAT_returnState crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id);
extern SAT_returnState route_pkt(tc_tm_pkt *pkt);

extern void cnv16_8(uint16_t from, uint8_t *to);
extern void cnv8_16(uint8_t *from, uint16_t *to);


//ToDo
//  check again if app_id and dest_id are ok.
//  assert, require.
//  finish definitions, types, subtypes, documentation and doc.
//  check again if finished segmentation

//Finito
//  check types definitions
//  add function definition in .h
//  check size and packet len. reconfigure for pkt len instead of pack.
//  check definition of n.
//  add pack in pack
//  what happens when new packet arrives, when the state is not free.
//  when to change iterator to next.
//  first tx packet, what header sould be.
//  check downlink, uplink subtypes.
//  what happens in timeout.
//  implement timeout and abort.
//  finish timeout.
//  check sequence numbers.
//  in tx when to make it FREE, maybe should ack every packet and then send it free.

SAT_returnState large_data_timeout();

SAT_returnState large_data_app(tc_tm_pkt *pkt);

SAT_returnState large_data_firstRx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_intRx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_lastRx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_retryRx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_standaloneRx_api(tc_tm_pkt *pkt);


SAT_returnState large_data_reportTx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_downlinkTx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_intTx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_retryTx_api(tc_tm_pkt *pkt);

SAT_returnState large_data_abort_api(tc_tm_pkt *pkt);

SAT_returnState large_data_updatePkt(tc_tm_pkt *pkt, uint16_t size, uint8_t subtype);

SAT_returnState large_data_downlinkPkt(tc_tm_pkt *pkt, uint16_t n, MS_sid sid, uint16_t dest_id);

SAT_returnState large_data_verifyPkt(tc_tm_pkt *pkt, uint16_t n, uint16_t dest_id);

SAT_returnState large_data_abortPkt(tc_tm_pkt *pkt, uint16_t dest_id, uint8_t subtype);

#endif