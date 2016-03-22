#include "../Inc/large_data_service.h"

SAT_returnState large_data_app(tc_tm_pkt *pkt) {
    SAT_returnState res;

    res = ld_api(pkt);
    ld_crt_pkt(tc_tm_pkt *pkt, res);

    return SATR_OK;
}

SAT_returnState ld_api(tc_tm_pkt *pkt) {

    if(ld_status.state == FREE && (pkt->dest_id == IAC || pkt->dest_id == GND) && pkt->ser_subtype == FIRST_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        sid = pkt->data[x];

        if( pkt->dest_id == IAC && sid != FOTOS) {
            return SATR_ERROR;
        } else if(  pkt->dest_id == GND && (sid != SU_SCRIPT_1 || sid != SU_SCRIPT_2 || sid != SU_SCRIPT_3 || sid != SU_SCRIPT_4 || sid != SU_SCRIPT_5 || sid != SU_SCRIPT_6 || sid != SU_SCRIPT_7)) {
            return SATR_ERROR;
        } else {
            return SATR_ERROR;
        }

        ld_status.app_id = pkt->dest_id;
        ld_status.sid = sid;
        ld_status.last_ld_seq = 0;

        mass_storage_store_api(sid, &file, pkt->data[MS_PKT_HDR], &size, &&ld_status.last_ld_seq);

        ld_status.file = file;
        ld_status.state = RECEIVING;

        //ld_status.timeout = time.now();

    } else if(ld_status.state == RECEIVING && (pkt->dest_id == IAC || pkt->dest_id == GND) && pkt->ser_subtype == IMD_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) {
            return SATR_ERROR;
        }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) {
            return SATR_ERROR;
        }

        ld_status.last_ld_seq++;

        mass_storage_store_api(TEMP, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);

        //ld_status.timeout = time.now();

    } else if(ld_status.state == RECEIVING && (pkt->dest_id == IAC || pkt->dest_id == GND) && pkt->ser_subtype == LAST_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) {
            return SATR_ERROR;
        }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) {
            return SATR_ERROR;
        }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;
    
    /*Starting TX*/
    } else if(ld_status.state == FREE && pkt->app_id == GND && pkt->ser_type == TC_MASS_STORAGE_SERVICE) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        sid = pkt->data[x];

        if( pkt->dest_id == IAC && sid != FOTOS) {
            return SATR_ERROR;
        } else if(  pkt->dest_id == GND && (sid != SU_SCRIPT_1 || sid != SU_SCRIPT_2 || sid != SU_SCRIPT_3 || sid != SU_SCRIPT_4 || sid != SU_SCRIPT_5 || sid != SU_SCRIPT_6 || sid != SU_SCRIPT_7)) {
            return SATR_ERROR;
        } else {
            return SATR_ERROR;
        }

        ld_status.app_id = pkt->dest_id;
        ld_status.sid = sid;
        ld_status.last_ld_seq = 0;

        mass_storage_downlink_api(uint8_t sid, uint32_t file, uint8_t *buf, &size, &ld_status.last_ld_seq);
        mass_storage_store_api(sid, &file, pkt->data[MS_PKT_HDR], &size, &ld_status.last_ld_seq);

        ld_status.file = file;
        ld_status.state = TRANSMITING;

        //ld_status.timeout = time.now();

    } else if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == ACC_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) {
            return SATR_ERROR;
        }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) {
            return SATR_ERROR;
        }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;
    
    } else if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == RETX_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) {
            return SATR_ERROR;
        }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) {
            return SATR_ERROR;
        }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;

    } else if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == ABORT_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) {
            return SATR_ERROR;
        }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) {
            return SATR_ERROR;
        }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;
    
    } else {
        return SATR_ERROR;
    }

    return SATR_OK;
}
