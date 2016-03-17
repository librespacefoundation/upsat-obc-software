#include "large_data_service.h"

OBC_returnStateTypedef large_data_app(tc_tm_pkt *pkt) {

    OBC_returnStateTypedef res;

    if(pkt->ser_subtype == FIRST_UPLINK)        { large_data_firstRx_api(pkt); } 
    else if(pkt->ser_subtype == IMD_UPLINK)     { large_data_intRx_api(pkt); } 
    else if(pkt->ser_subtype == LAST_UPLINK)    { large_data_lastRx_api(pkt); } 

    else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == REPORT) { large_data_reportTx_api(pkt); } 
    else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == DOWNLINK) { large_data_downlinkTx_api(pkt); } 
    else if(pkt->ser_subtype == ACC) { large_data_intTx_api(pkt); } 
    else if(pkt->ser_subtype == RETRANSMIT) { large_data_retryTx_api(pkt); } 
    else if(pkt->ser_subtype == ABORT) { large_data_abortTx_api(pkt); }

    return R_ERROR;
}


OBC_returnStateTypedef large_data_verifyPkt_api(tc_tm_pkt *pkt, uint16_t n, uint16_t dest_id) {

    pkt = get_pkt(NORMAL)
    if(pkt == NULL) { return R_ERROR; }
    crt_pkt(pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, ACC_REPORT, dest_id); //what dest_id ?

    pkt->data[0] = n;

    return R_OK;
}

/*downlink*/
OBC_returnStateTypedef large_data_firstRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t app_id;
    uint16_t size;
    MS_sid sid;

    ASSERT(pkt != NULL && pkt->data != NULL)
    REQUIRE(LD_status.state == FREE);

    sid = pkt->data[1];
    ld_num = pkt->data[0];
    app_id = pkt->dest_id;
    size = pkt->len;

    REQUIRE(app_id == IAC || app_id == GND) { return R_ERROR; } 
    REQUIRE(ld_num == 0);
    REQUIRE(size > 0);
    REQUIRE((app_id == IAC && sid == FOTOS) || (app_id == GND && (sid =< SU_SCRIPT_7 )) { return R_ERROR; } 

    LD_status.app_id = app_id;
    LD_status.sid = sid;
    LD_status.ld_num = ld_num;
    LD_status.state = RECEIVING;

    mass_storage_store_api(sid, 0, pkt->data[MS_PKT_HDR], &size, LD_status.ld_num);

    LD_status.timeout = time.now();
    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id , FIRST_UPLINK);
    route_pkt(temp_pkt);
    
    return R_OK;
}

OBC_returnStateTypedef large_data_intRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;

    ASSERT(pkt != NULL && pkt->data != NULL)
    REQUIRE(LD_status.state == RECEIVING) { return R_ERROR; }
    REQUIRE(LD_status.app_id != pkt->dest_id) { return R_ERROR; }
    REQUIRE(pkt->dest_id == IAC || pkt->dest_id == GND) { return R_ERROR; } 

    ld_num = pkt->data[0];
    size = pkt->len;

    REQUIRE(size > 0);
    REQUIRE(LD_status.ld_num == ld_num - 1) { return R_ERROR; }

    LD_status.ld_num = ld_num;

    mass_storage_store_api(sid, 0, pkt->data[MS_PKT_HDR], &size, LD_status.ld_num);

    LD_status.timeout = time.now();
    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id , IMD_UPLINK);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_lastRx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t size;

    ASSERT(pkt != NULL && pkt->data != NULL)
    REQUIRE(LD_status.state == RECEIVING) { return R_ERROR; }
    REQUIRE(LD_status.app_id != pkt->dest_id) { return R_ERROR; }
    REQUIRE(pkt->dest_id == IAC || pkt->dest_id == GND) { return R_ERROR; } 

    ld_num = pkt->data[0];
    size = pkt->len;

    REQUIRE(size > 0);
    REQUIRE(LD_status.ld_num == ld_num - 1) { return R_ERROR; }

    LD_status.ld_num = ld_num;

    mass_storage_store_api(sid, LAST_PART, pkt->data[MS_PKT_HDR], &size, LD_status.ld_num);

    LD_status.state = FREE;
    LD_status.ld_num = 0;
    LD_status.timeout = 0;

    //return R_OK;
    tc_tm_pkt *temp_pkt;

    large_data_verifyPkt_api(temp_pkt, LD_status.ld_num, LD_status.app_id , LAST_UPLINK);
    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_reportTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t app_id;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;

    ASSERT(pkt != NULL && pkt->data != NULL)

    app_id = pkt->dest_id; //check if this is ok

    sid = pkt->data[0];

    REQUIRE(LD_status.state == FREE && app_id == GND) { return R_ERROR; }
    REQUIRE(sid == FOTOS || sid == FOTOS || sid == FOTOS || sid == FOTOS || ){ return R_ERROR; } 
    REQUIRE(mode < LAST_MODE);

    LD_status.fcurr = 0;

    size = MS_somt;

    temp_pkt = get_pkt(EXTENDED);
    if(temp_pkt == NULL) { return R_ERROR; }

    temp_pkt->data[0] = 0;
    temp_pkt->data[1] = sid;
    temp_pkt->data[2] = 0;

    res = mass_storage_report_api(sid, temp_pkt->data[3], &size, &LD_status.fnext);

    temp_pkt->len = size;

    if(res == R_EOT) {
        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, ONE_PACKET, dest_id); //what dest_id ?
    } else {

        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, FIRST_PACKET, dest_id);

        LD_status.app_id = app_id;
        LD_status.sid = sid;
        LD_status.ld_num = ld_num;

        LD_status.state = TRANSMITING;

        LD_status.timeout = time.now();
    }

    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_downlinkTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t app_id;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    uint32_t size;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;

    ASSERT(pkt != NULL && pkt->data != NULL)

    app_id = pkt->dest_id; //check if this is ok

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

    REQUIRE(LD_status.state == FREE && app_id == GND) { return R_ERROR; }
    REQUIRE(sid == FOTOS || sid == FOTOS || sid == FOTOS || sid == FOTOS || ){ return R_ERROR; } 
    REQUIRE(mode < LAST_MODE);

    LD_status.fcurr = 0;

    size = MS_somt;

    temp_pkt = get_pkt(EXTENDED);
    if(temp_pkt == NULL) { return R_ERROR; }

    temp_pkt->data[0] = 0;
    temp_pkt->data[1] = sid;
    temp_pkt->data[2] = 0;

    res = mass_storage_downlink_api(sid, mode, from, to, temp_pkt->data[3], &size, &LD_status.fnext);

    temp_pkt->len = size;

    if(res == R_EOT) {
        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, ONE_PACKET, dest_id); //what dest_id ?
    } else {

        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, FIRST_PACKET, dest_id);

        LD_status.app_id = app_id;
        LD_status.sid = sid;
        LD_status.mode = mode;
        LD_status.from = from;
        LD_status.to = to;
        LD_status.ld_num = ld_num;

        LD_status.state = TRANSMITING;

        LD_status.timeout = time.now();
    }

    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_intTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t app_id;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    uint32_t size;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;

    ld_num = pkt->data[0];

    ASSERT(pkt != NULL && pkt->data != NULL)
    REQUIRE(LD_status.app_id != pkt->dest_id) { return R_ERROR; }
    REQUIRE(LD_status.state == TRANSMITING_DOWNLINK && app_id == GND) { return R_ERROR; }
    REQUIRE(LD_status.ld_num == ld_num - 1) { return R_ERROR; }

    size = MS_somt;

    temp_pkt = get_pkt(EXTENDED);
    if(temp_pkt == NULL) { return R_ERROR; }

    temp_pkt->data[0] = LD_status.ld_num + 1;

    if(LD_status.txType == DOWNLINK) { 
        res = mass_storage_downlink_api(LD_status.sid, LD_status.mode, LD_status.from, LD_status.to, temp_pkt->data[3], &size, &LD_status.fnext);
    } else if(LD_status.txType == REPORT) {
        res = mass_storage_report_api(LD_status.sid, temp_pkt->data[3], &size, &LD_status.fnext);
    } //what to do with else

    temp_pkt->len = size;

    if(res == R_EOT) {
        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, LAST_PACKET, dest_id); //what dest_id ?
    } else {

        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, INT_PACKET, dest_id);

        LD_status.ld_num++;

        LD_status.timeout = time.now();
    }

    route_pkt(temp_pkt);

    return R_OK;
}


OBC_returnStateTypedef large_data_retryTx_api(tc_tm_pkt *pkt) {

    uint16_t ld_num;
    uint16_t app_id;
    uint16_t size;
    uint32_t from;
    uint32_t to;
    uint32_t size;
    MS_sid sid;
    MS_mode mode;
    OBC_returnStateTypedef res;

    ld_num = pkt->data[0];

    ASSERT(pkt != NULL && pkt->data != NULL)
    REQUIRE(LD_status.app_id != pkt->dest_id) { return R_ERROR; }
    REQUIRE(LD_status.state == TRANSMITING_DOWNLINK && app_id == GND) { return R_ERROR; }
    REQUIRE(LD_status.ld_num == ld_num) { return R_ERROR; }

    size = MS_somt;

    temp_pkt = get_pkt(EXTENDED);
    if(temp_pkt == NULL) { return R_ERROR; }

    temp_pkt->data[0] = LD_status.ld_num;

    if(LD_status.txType == DOWNLINK) { 
        res = mass_storage_downlink_api(LD_status.sid, LD_status.mode, LD_status.from, LD_status.to, temp_pkt->data[3], &size, &LD_status.fnext);
    } else if(LD_status.txType == REPORT) {
        res = mass_storage_report_api(LD_status.sid, temp_pkt->data[3], &size, &LD_status.fnext);
    } //what to do with else

    temp_pkt->len = size;

    if(res == R_EOT) {
        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, LAST_PACKET, dest_id); //what dest_id ?
    } else {

        crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_LARGE_DATA_SERVICE, INT_PACKET, dest_id);

        LD_status.timeout = time.now();
    }

    route_pkt(temp_pkt);

    return R_OK;
}

OBC_returnStateTypedef large_data_abortTx_api(tc_tm_pkt *pkt) {

    LD_status.state = FREE;

    LD_status.timeout = 0;

    return R_OK;
}
