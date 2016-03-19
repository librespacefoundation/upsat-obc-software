#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"
#include "mass_storage_service.h"

#define MAX_LD_PKT_DATA MAX_EX_PKT_DATA - LD_PKT_HDR

struct _ld_status {
    uint8_t state;          /*service state machine, state variable*/
    uint8_t sid;            /*sid */
    uint8_t app_id;         /*destination app id*/
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
}LD_status;

//ToDo
//  check again if app_id and dest_id are ok.
//  assert, require.
//  finish definitions, types, subtypes, documentation and doc.
//  finish segmentation
//  add pack in pack

//Finito
//  what happens when new packet arrives, when the state is not free.
//  when to change iterator to next.
//  first tx packet, what header sould be.
//  check downlink, uplink subtypes.
//  what happens in timeout.
//  implement timeout and abort.
//  finish timeout.
//  check sequence numbers.
//  in tx when to make it FREE, maybe should ack every packet and then send it free.

OBC_returnStateTypedef large_data_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef large_data_startReport_api(uint8_t sid, tc_tm_pkt *pkt);

OBC_returnStateTypedef large_data_startDownlink_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, tc_tm_pkt *pkt);

#endif