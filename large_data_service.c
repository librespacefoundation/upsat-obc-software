#include "large_data_service.h"

OBC_returnStateTypedef large_data_app(tc_tm_pkt *pkt) {

    OBC_returnStateTypedef res;

    if(pkt->ser_subtype == TC_LD_FIRST_UPLINK)              { large_data_firstRx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_INT_UPLINK)           { large_data_intRx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_LAST_UPLINK)          { large_data_lastRx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_REPEATED_UPLINK)      { large_data_retryRx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_STANDALONE_UPLINK)    { large_data_standaloneRx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_ABORT_SE_UPLINK)      { large_data_abort_api(pkt); } 

    else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == TC_MS_REPORT)   { large_data_reportTx_api(pkt); } 
    else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == TC_MS_DOWNLINK) { large_data_downlinkTx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_ACK_DOWNLINK)         { large_data_intTx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_REPEAT_DOWNLINK)      { large_data_retryTx_api(pkt); } 
    else if(pkt->ser_subtype == TC_LD_ABORT_RE_DOWNLINK)    { large_data_abort_api(pkt); }

    return R_OK;
}

/*downlink*/
OBC_returnStateTypedef large_data_firstRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    TC_TM_app_id app_id;
    MS_sid sid;
    tc_tm_pkt *temp_pkt;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; } 
    if(!C_ASSERT(LD_status.state == LD_STATE_FREE) == true) { 
        large_data_abortPkt_api(temp_pkt, pkt->dest_id, TC_LD_ABORT_RE_UPLINK); 
        route_pkt(temp_pkt);
        return R_OK; 
    }

    cnv8_16(&ld_num, pkt->data[0]);
    sid = pkt->data[2];

    app_id = pkt->dest_id;
    size = pkt->len-LD_PKT_DATA_HDR_SIZE; //ldata headers

    if(!C_ASSERT(app_id == IAC_APP_ID || app_id == GND_APP_ID) == true)                                             { return R_ERROR; }
    if(!C_ASSERT(ld_num == 0) == true)                                                                              { return R_ERROR; }
    if(!C_ASSERT(size > 0) == true)                                                                                 { return R_ERROR; } 
    if(!C_ASSERT((app_id == IAC_APP_ID && sid == FOTOS) || (app_id == GND_APP_ID && sid <= SU_SCRIPT_7 )) == true) { return R_ERROR; } 

    large_data_state();
    LD_status.app_id = app_id;
    LD_status.sid = sid;
    LD_status.ld_num = ld_num;
    LD_status.state = LD_STATE_RECEIVING;
    LD_status.started = time.now();

    mass_storage_store_api(sid, 0, pkt->data[LD_PKT_DATA_HDR_SIZE], &size, LD_status.ld_num);

    LD_status.timeout = time.now();
    //return R_OK;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_intRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    TC_TM_app_id app_id;
    MS_sid sid;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { return R_ERROR; }
    if(!C_ASSERT(LD_status.state == LD_STATE_RECEIVING) == true)                    { return R_ERROR; }
    if(!C_ASSERT(LD_status.app_id != pkt->dest_id) == true)                         { return R_ERROR; }
    if(!C_ASSERT(pkt->dest_id == IAC_APP_ID || pkt->dest_id == GND_APP_ID) == true) { return R_ERROR; }

    cnv8_16(&ld_num, pkt->data[0]);
    sid = pkt->data[2];

    app_id = pkt->dest_id;
    size = pkt->len-LD_PKT_DATA_HDR_SIZE; //ldata headers

    if(!C_ASSERT(size > 0) == true)                     { return R_ERROR; }
    if(!C_ASSERT(LD_status.ld_num == ld_num) == true)   { return R_ERROR; }
    if(!C_ASSERT(LD_status.sid == sid) == true)         { return R_ERROR; }

    LD_status.ld_num = ld_num;

    mass_storage_store_api(sid, 0, pkt->data[LD_PKT_DATA_HDR_SIZE], &size, LD_status.ld_num);

    LD_status.timeout = time_now();
    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_lastRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    TC_TM_app_id app_id;
    MS_sid sid;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { return R_ERROR; }
    if(!C_ASSERT(LD_status.state == LD_STATE_RECEIVING) == true)                    { return R_ERROR; }
    if(!C_ASSERT(LD_status.app_id != pkt->dest_id) == true)                         { return R_ERROR; }
    if(!C_ASSERT(pkt->dest_id == IAC_APP_ID || pkt->dest_id == GND_APP_ID) == true) { return R_ERROR; }

    cnv8_16(&ld_num, pkt->data[0]);
    sid = pkt->data[2];

    app_id = pkt->dest_id;
    size = pkt->len-LD_PKT_DATA_HDR_SIZE; //ldata headers

    if(!C_ASSERT(size > 0) == true)                     { return R_ERROR; }
    if(!C_ASSERT(LD_status.ld_num == ld_num) == true)   { return R_ERROR; }
    if(!C_ASSERT(LD_status.sid == sid) == true)         { return R_ERROR; }

    LD_status.ld_num = ld_num;

    mass_storage_store_api(sid, LAST_PART, pkt->data[LD_PKT_DATA_HDR_SIZE], &size, LD_status.ld_num);

    LD_status.state = LD_STATE_FREE;
    LD_status.ld_num = 0;
    LD_status.timeout = 0;
    LD_status.started = 0;

    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_retryRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    TC_TM_app_id app_id;
    MS_sid sid;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { return R_ERROR; }
    if(!C_ASSERT(LD_status.state == LD_STATE_RECEIVING) == true)                    { return R_ERROR; }
    if(!C_ASSERT(LD_status.app_id != pkt->dest_id) == true)                         { return R_ERROR; }
    if(!C_ASSERT(pkt->dest_id == IAC_APP_ID || pkt->dest_id == GND_APP_ID) == true) { return R_ERROR; }

    cnv8_16(&ld_num, pkt->data[0]);
    sid = pkt->data[2];

    app_id = pkt->dest_id;
    size = pkt->len-LD_PKT_DATA_HDR_SIZE; //ldata headers

    if(!C_ASSERT(size > 0) == true)                         { return R_ERROR; }
    if(!C_ASSERT(LD_status.ld_num == ld_num - 1) == true)   { return R_ERROR; }
    if(!C_ASSERT(LD_status.sid == sid) == true)             { return R_ERROR; }

    LD_status.ld_num = ld_num;

    mass_storage_store_api(sid, 0, pkt->data[LD_PKT_DATA_HDR_SIZE], &size, LD_status.ld_num);

    LD_status.timeout = time_now();
    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_standaloneRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    TC_TM_app_id app_id;
    MS_sid sid;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    cnv8_16(&ld_num, pkt->data[0]);
    sid = pkt->data[2];

    app_id = pkt->dest_id;
    size = pkt->len-LD_PKT_DATA_HDR_SIZE; //ldata headers

    if(!C_ASSERT(app_id == GND_APP_ID) == true)                             { return R_ERROR; }
    if(!C_ASSERT(ld_num == 0) == true)                                      { return R_ERROR; }
    if(!C_ASSERT(size > 0) == true)                                         { return R_ERROR; }
    if(!C_ASSERT(app_id == GND_APP_ID && (sid <= SU_SCRIPT_7 )) == true)    { return R_ERROR; }

    mass_storage_store_api(sid, LAST_PART, pkt->data[LD_PKT_DATA_HDR_SIZE], &size, 0);

    LD_status.timeout = time_now();
    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, 0, LD_status.app_id);
    route_pkt(temp_pkt);
    
    return R_OK;
}

OBC_returnStateTypedef large_data_reportTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    uint8_t subtype;
    TC_TM_app_id app_id;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;
    tc_tm_pkt *temp_pkt;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    app_id = pkt->dest_id; //check if this is ok

    sid = pkt->data[0];

    if(!C_ASSERT(LD_status.state == LD_STATE_FREE && app_id == GND_APP_ID) == true) {
        large_data_abortPkt_api(temp_pkt, pkt->dest_id, TC_LD_ABORT_SE_DOWNLINK); 
        route_pkt(temp_pkt);
        return R_OK; 
    }

    if(!C_ASSERT(sid == FOTOS || sid == EVENT_LOG || sid == SU_LOG) == true)    { return R_ERROR; }
    if(!C_ASSERT(mode < LAST_MODE) == true)                                     { return R_ERROR; }

    LD_status.fcurr = 0;

    size = LD_PKT_DATA;

    large_data_downlinkPkt(temp_pkt, 0, sid, app_id);

    res = mass_storage_report_api(sid, temp_pkt->data[LD_PKT_DATA_HDR_SIZE], &size, &LD_status.fnext);

    temp_pkt->len = size;

    if(res == R_EOT) { subtype = TC_LD_STANDALONE_DOWNLINK; }
    else {

        subtype = TC_LD_FIRST_DOWNLINK;

        LD_status.app_id = app_id;
        LD_status.sid = sid;
        LD_status.ld_num = 1;
        LD_status.lpacket_flag = false;

        LD_status.state = LD_STATE_TRANSMITING;
        LD_status.txType = LD_STATE_REPORT;
        LD_status.started = time_now();

        LD_status.timeout = time_now();
    }

    large_data_updatePkt(temp_pkt, size, subtype);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_downlinkTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    uint8_t subtype;
    TC_TM_app_id app_id;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;
    tc_tm_pkt *temp_pkt;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    app_id = pkt->dest_id; //check if this is ok
    sid = pkt->data[0];
    mode = pkt->data[1];

    cnv8_32(&from, pkt->data[2], pkt->data[3], pkt->data[4], pkt->data[5]);
    cnv8_32(&to, pkt->data[6], pkt->data[7], pkt->data[8], pkt->data[9]);

    if(!C_ASSERT(LD_status.state == LD_STATE_FREE && app_id == GND_APP_ID) == true) {
        large_data_abortPkt_api(temp_pkt, pkt->dest_id, TC_LD_ABORT_SE_DOWNLINK); 
        route_pkt(temp_pkt);
        return R_OK; 
    }

    if(!C_ASSERT(sid == FOTOS || sid == EVENT_LOG || sid == SU_LOG) == true)    { return R_ERROR; }
    if(!C_ASSERT(mode < LAST_MODE) == true)                                     { return R_ERROR; }

    LD_status.fcurr = 0;

    size = LD_PKT_DATA;

    large_data_downlinkPkt(temp_pkt, 0, sid, app_id);

    res = mass_storage_downlink_api(sid, mode, from, to, temp_pkt->data[LD_PKT_DATA_HDR_SIZE], &size, &LD_status.fnext);

    temp_pkt->len = size;

    if(res == R_EOT) { subtype = TC_LD_STANDALONE_DOWNLINK; } 
    else {

        subtype = TC_LD_FIRST_DOWNLINK;

        LD_status.app_id = app_id;
        LD_status.sid = sid;
        LD_status.mode = mode;
        LD_status.from = from;
        LD_status.to = to;
        LD_status.ld_num = 1;
        LD_status.lpacket_flag = false;

        LD_status.state = LD_STATE_TRANSMITING;
        LD_status.txType = LD_STATE_DOWNLINK;
        LD_status.started = time_now();

        LD_status.timeout = time_now();
    }

    large_data_updatePkt(temp_pkt, size, subtype);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_intTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    uint32_t fcurr;
    uint8_t subtype;
    uint32_t fnext;
    TC_TM_app_id app_id;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;
    tc_tm_pkt *temp_pkt;

    ld_num = pkt->data[0];

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                                             { return R_ERROR; }
    if(!C_ASSERT(LD_status.app_id != pkt->dest_id) == true)                                             { return R_ERROR; }
    if(!C_ASSERT(LD_status.state == LD_STATE_TRANSMITING && LD_status.app_id == GND_APP_ID) == true)    { return R_ERROR; }
    if(!C_ASSERT(LD_status.ld_num == ld_num - 1) == true)                                               { return R_ERROR; }

    if(LD_status.lpacket_flag == true) {
        LD_status.lpacket_flag = false;
        LD_status.state = LD_STATE_FREE;
        LD_status.started = time_now();
        LD_status.timeout = 0;

        return R_OK;
    }

    size = LD_PKT_DATA;

    fnext = LD_status.fnext;

    large_data_downlinkPkt(temp_pkt, LD_status.ld_num + 1, LD_status.sid, LD_status.app_id);

    if(LD_status.txType == LD_STATE_DOWNLINK)    { res = mass_storage_downlink_api(LD_status.sid, LD_status.mode, LD_status.from, LD_status.to, temp_pkt->data[LD_PKT_DATA_HDR_SIZE], &size, &LD_status.fnext); }
    else if(LD_status.txType == LD_STATE_REPORT) { res = mass_storage_report_api(LD_status.sid, temp_pkt->data[LD_PKT_DATA_HDR_SIZE], &size, &LD_status.fnext); } //what to do with else

    if(res == R_EOT) {
        LD_status.lpacket_flag = true;
        subtype = TC_LD_LAST_DOWNLINK;
    } else {
        subtype = TC_LD_INT_DOWNLINK;
        LD_status.fcurr = fnext;
        LD_status.ld_num++;    
    }

    large_data_updatePkt(temp_pkt, size, subtype);
    LD_status.timeout = time_now();
    route_pkt(temp_pkt);

    return R_OK;
}


OBC_returnStateTypedef large_data_retryTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint32_t from;
    uint32_t to;
    uint16_t size;
    uint32_t fnext;
    TC_TM_app_id app_id;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;
    tc_tm_pkt *temp_pkt;

    ld_num = pkt->data[0];

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                                             { return R_ERROR; }
    if(!C_ASSERT(LD_status.app_id != pkt->dest_id) == true)                                             { return R_ERROR; }
    if(!C_ASSERT(LD_status.state == LD_STATE_TRANSMITING && LD_status.app_id == GND_APP_ID) == true)    { return R_ERROR; }
    if(!C_ASSERT(LD_status.ld_num == ld_num - 1) == true)                                               { return R_ERROR; }

    size = LD_PKT_DATA;

    fnext = LD_status.fcurr;

    large_data_downlinkPkt(temp_pkt, LD_status.ld_num, LD_status.sid, LD_status.app_id);

    if(LD_status.txType == LD_STATE_DOWNLINK)     { res = mass_storage_downlink_api(LD_status.sid, LD_status.mode, LD_status.from, LD_status.to, temp_pkt->data[3], &size, &fnext); } 
    else if(LD_status.txType == LD_STATE_REPORT)  { res = mass_storage_report_api(LD_status.sid, temp_pkt->data[LD_PKT_DATA_HDR_SIZE], &size, &fnext); }

    large_data_updatePkt(temp_pkt, size, TC_LD_REPEATED_DOWNLINK);

    LD_status.timeout = time_now();

    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_updatePkt(tc_tm_pkt *pkt, uint16_t size, uint8_t subtype) {

    pkt->ser_subtype = subtype;
    pkt->len = size+LD_PKT_DATA_HDR_SIZE;

    return R_OK;
}

OBC_returnStateTypedef large_data_downlinkPkt(tc_tm_pkt *pkt, uint16_t n, MS_sid sid, uint16_t dest_id) {

    pkt = get_pkt(NORMAL);
    if(!C_ASSERT(pkt != NULL) == true) { return R_ERROR; }
    crt_pkt(pkt, OBC_APP_ID, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, 0, dest_id); //what dest_id ?

    cnv16_8(n, pkt->data[0]);
    pkt->data[2] = sid;

    return R_OK;
}

OBC_returnStateTypedef large_data_verifyPkt(tc_tm_pkt *pkt, uint16_t n, uint16_t dest_id) {

    pkt = get_pkt(EXTENDED);
    if(!C_ASSERT(pkt != NULL) == true) { return R_ERROR; }
    crt_pkt(pkt, OBC_APP_ID, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, TC_LD_ACK_UPLINK, dest_id);

    cnv16_8(n, pkt->data[0]);

    pkt->len = 2;

    return R_OK;
}

OBC_returnStateTypedef large_data_abortPkt(tc_tm_pkt *pkt, uint16_t dest_id, uint8_t subtype) {

    pkt = get_pkt(NORMAL);
    if(!C_ASSERT(pkt != NULL) == true) { return R_ERROR; }
    crt_pkt(pkt, OBC_APP_ID, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, subtype, dest_id);

    pkt->data[0] = R_ALREADY_SERVICING;

    pkt->len = 1;

    return R_OK;
}

OBC_returnStateTypedef large_data_abort(tc_tm_pkt *pkt) {

    LD_status.state = LD_STATE_FREE;
    LD_status.ld_num = 0;
    LD_status.timeout = 0;
    LD_status.started = 0;

    return R_OK;
}

OBC_returnStateTypedef large_data_timeout() {

    tc_tm_pkt *temp_pkt;

    if(LD_status.state = LD_STATE_TRANSMITING) {
        large_data_abortPkt_api(temp_pkt, LD_status.app_id, TC_LD_ABORT_SE_DOWNLINK); 
        route_pkt(temp_pkt);
        return R_OK; 
    }
    else if(LD_status.state = LD_STATE_RECEIVING) {
        large_data_abortPkt_api(temp_pkt, LD_status.app_id, TC_LD_ABORT_RE_UPLINK); 
        route_pkt(temp_pkt);
        return R_OK; 
    }

    LD_status.state = LD_STATE_FREE;
    LD_status.ld_num = 0;
    LD_status.timeout = 0;
    LD_status.started = 0;

    return R_OK;
}
