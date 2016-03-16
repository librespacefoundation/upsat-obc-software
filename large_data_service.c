#include "large_data_service.h"

OBC_returnStateTypedef large_data_app(tc_tm_pkt *pkt) {

    OBC_returnStateTypedef res;
    
    if(ld_status.state == FREE && pkt->ser_subtype == FIRST_DOWNLINK) {
        n = pkt->data[0];
        res = large_data_firstRx_api(pkt);
        large_data_verifyPkt_api(temp_pkt, n, pkt->dest_id, res, FIRST_DOWNLINK);
        route_pkt(temp_pkt);

    } else if(ld_status.state == RECEIVING && (pkt->dest_id == IAC || pkt->dest_id == GND) && pkt->ser_subtype == IMD_DOWNLINK) {
        n = pkt->data[0];
        res = large_data_intRx_api(pkt);
        large_data_verifyPkt_api(temp_pkt, n, pkt->dest_id, res, IMD_DOWNLINK);
        route_pkt(temp_pkt);

    } else if(ld_status.state == RECEIVING && (pkt->dest_id == IAC || pkt->dest_id == GND) && pkt->ser_subtype == LAST_DOWNLINK) {
        n  = pkt->data[0];
        res = large_data_finalRx_api(pkt);
        large_data_verifyPkt_api(temp_pkt, n, res, pkt->dest_id, IMD_DOWNLINK);
        route_pkt(temp_pkt);

    } else if(ld_status.state == FREE && pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == REPORT) {

        sid = pkt->data[0];

        large_data_reportTx_api(sid, mode, from, to, pkt); //finish fun
        large_data_pkt_api(pkt_out, 1, res, IMD_DOWNLINK);
        //mass_storage_report_api(sid, pkt_out->data[], MAX);
        route_pkt(temp_pkt);
        //need to find state for report tx

    } else if(ld_status.state == FREE && pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == DOWNLINK) {

        sid = pkt->data[0];

        if(sid == LOG)
        large_data_reportTx_api(sid, mode, from, to, pkt); //finish fun
        large_data_pkt_api(pkt_out, 1, res, IMD_DOWNLINK);
        //mass_storage_report_api(sid, pkt_out->data[], MAX);
        route_pkt(temp_pkt);
        //need to find state for report tx

    } else if(ld_status.state == TRANSMITING_REPORT && (pkt->ser_subtype == ACC || pkt->ser_subtype == RETRANSMIT)) {

        sid = pkt->data[0];

        large_data_pkt_api(pkt_out, n, res, IMD_DOWNLINK);
        large_data_reportTx_api(sid, mode, from, to, pkt); //finish fun
        //mass_storage_report_api(sid, pkt_out->data[], MAX);
        route_pkt(temp_pkt);
        //need to find state for report tx
    } else if(ld_status.state == TRANSMITING_LOGS && (pkt->ser_subtype == ACC || pkt->ser_subtype == RETRANSMIT)) {

    } else if(ld_status.state == TRANSMITING_LFILE && (pkt->ser_subtype == ACC || pkt->ser_subtype == RETRANSMIT))  {

    } else if(ld_status.state == TRANSMITING && pkt->ser_subtype == ABORT) {

    }

    return R_ERROR;
}


OBC_returnStateTypedef large_data_verifyPkt_api(tc_tm_pkt *pkt, uint16_t n,uint16_t dest_id, OBC_returnStateTypedef res) {

    temp_pkt = get_pkt(NORMAL);
    if(temp_pkt == NULL) { return R_ERROR; }
    crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, ACC_REPORT, dest_id); //what dest_id ?
    
    temp_pkt->data[0] = n;
    
    return R_OK;
}

OBC_returnStateTypedef large_data_pkt_api(tc_tm_pkt *pkt) {

    temp_pkt = get_pkt(EXTENDED);
    if(temp_pkt == NULL) { return R_ERROR; }
    crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, ACC_REPORT, pkt->dest_id);
    
    temp_pkt->data[0] = n;
    
    return R_OK;
}

/*downlink*/
OBC_returnStateTypedef large_data_firstRx_api(tc_tm_pkt *pkt) {

    uint32_t file;
    uint16_t size = MAX_LD_PKT_DATA, part = 0;
    uint8_t sid;

    if(ld_status.state != FREE && (pkt->dest_id != IAC || pkt->dest_id != GND)) { return R_ERROR; } 

    sid = pkt->data[x];
    ld_status.seq_num = pkt->data[x];

    if( pkt->dest_id == IAC && sid != FOTOS) { return R_ERROR; } 
    else if(  pkt->dest_id == GND && (sid != SU_SCRIPT_1 || sid != SU_SCRIPT_2 || sid != SU_SCRIPT_3 || sid != SU_SCRIPT_4 || sid != SU_SCRIPT_5 || sid != SU_SCRIPT_6 || sid != SU_SCRIPT_7)) {
        return R_ERROR;
    } else { return R_ERROR; }

    ld_status.app_id = pkt->dest_id;
    ld_status.sid = sid;
    ld_status.last_ld_seq = 0;

    mass_storage_storeLargeFile_api(sid, &file, pkt->data[MS_PKT_HDR], &size, &ld_status.last_ld_seq);

    ld_status.file = file;
    ld_status.state = RECEIVING;

    //ld_status.timeout = time.now();
    return R_OK;
}

OBC_returnStateTypedef large_data_intRx_api(tc_tm_pkt *pkt) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) { return R_ERROR; }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) { return R_ERROR; }

        ld_status.last_ld_seq++;

        mass_storage_store_api(TEMP, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);

        //ld_status.timeout = time.now();
}

 OBC_returnStateTypedef large_data_finalRx_api(tc_tm_pkt *pkt) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) { return R_ERROR; }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) { return R_ERROR; }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);

        ld_status.state = FREE;
        ld_status.timeout = 0;

    } else { return R_ERROR; }

    return R_OK;
}

OBC_returnStateTypedef large_data_firstTx_api(tc_tm_pkt *pkt) {

    
    if(ld_status.state == FREE) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        sid = pkt->data[0];

        mode = pkt->data[1];

        //cnv
        from = pkt->data[2];
        from = pkt->data[3];
        from = pkt->data[4];
        from = pkt->data[5];

        to = pkt->data[6];
        to = pkt->data[7];
        to = pkt->data[8];
        to = pkt->data[9];

        if( pkt->dest_id == IAC && sid != FOTOS) { return R_ERROR; } 
        else if(  pkt->dest_id == GND && (sid != SU_SCRIPT_1 || sid != SU_SCRIPT_2 || sid != SU_SCRIPT_3 || sid != SU_SCRIPT_4 || sid != SU_SCRIPT_5 || sid != SU_SCRIPT_6 || sid != SU_SCRIPT_7)) {
            return R_ERROR;
        } else { return R_ERROR; }

        ld_status.app_id = pkt->dest_id;
        ld_status.sid = sid;
        ld_status.last_ld_seq = 0;

        mass_storage_downlink_api(uint8_t sid, uint32_t file, uint8_t *buf, &size, &ld_status.last_ld_seq);
        mass_storage_store_api(sid, &file, pkt->data[MS_PKT_HDR], &size, &ld_status.last_ld_seq);

        ld_status.file = file;
        ld_status.state = TRANSMITING;

        //ld_status.timeout = time.now();

    }

    return R_ERROR;
}

OBC_returnStateTypedef large_data_intTx_api(tc_tm_pkt *pkt) {

    if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == ACC_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) { return R_ERROR; }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) { return R_ERROR; }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;

    } 
    return R_ERROR;
}

OBC_returnStateTypedef large_data_finalTx_api(tc_tm_pkt *pkt) {

    if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == ACC_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) { return R_ERROR; }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) { return R_ERROR; }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;

    } 
    return R_ERROR;
}

OBC_returnStateTypedef large_data_retryTx_api(tc_tm_pkt *pkt) {

    if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == RETX_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) { return R_ERROR; }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) { return R_ERROR; }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;

    }
}

OBC_returnStateTypedef large_data_abortTx_api(tc_tm_pkt *pkt) {

    if(ld_status.state == TRANSMITING && pkt->dest_id == GND && pkt->ser_subtype == ABORT_DOWNLINK) {

        uint32_t file;
        uint16_t size = MAX_LD_PKT_DATA, part = 0;
        uint8_t sid;

        if(ld_status.app_id != pkt->dest_id) { return R_ERROR; }

        ld_seq = pkt->data[x];

        if(ld_status.last_ld_seq != ld_seq - 1) { return R_ERROR; }

        ld_status.last_ld_seq++;

        mass_storage_store_api(ld_status.sid, &ld_status.file, pkt->data[LD_PKT_HDR], &size, &ld_status.last_ld_seq);
        mass_storage_move_api(ld_status.sid, ld_status.file, 0);

        ld_status.state = FREE;
        ld_status.timeout = 0;

    } else { return R_ERROR; }

    return R_OK;
}
