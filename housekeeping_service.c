#include "housekeeping_service.h"

void hk_SCH() {

    tc_tm_pkt pkt;
  
    hk_crt_pkt_TC(&pkt, EPS, 1);
    route_pkt(&pkt);
    hk_crt_pkt_TC(&pkt, COMMS, 1);
    route_pkt(&pkt);
    //delay(59) sec;
    hk_crt_pkt_TM(&pkt, GND, 4);
    route_pkt(&pkt);
    clear_wod();
}

void clear_wod() {
        obc_status.batt_curr = 0;
        obc_status.batt_volt = 0;
        obc_status.bus_3v3_curr = 0;
        obc_status.bus_5v_curr = 0;
        obc_status.temp_eps = 0;
        obc_status.temp_batt = 0;
        obc_status.temp_comms = 0;
}

OBC_returnStateTypedef hk_app(tc_tm_pkt *pkt) {

    C_ASSERT(pkt != NULL && pkt->data != NULL) { return R_ERROR; }

    if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE &&  pkt->ser_subtype == TC_HK_REPORT_PARAMETERS) {
        hk_crt_pkt_TM(pkt, pkt->dest_id, pkt->data);
        route_pkt(pkt);
    } else if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE &&  pkt->ser_subtype == TC_HK_PARAMETERS_REPORT) {
        if(pkt->app_id == EPS) {
            obc_status.batt_curr = pkt->data[1];
            obc_status.batt_volt = pkt->data[2];
            obc_status.bus_3v3_curr = pkt->data[3];
            obc_status.bus_5v_curr = pkt->data[4];
            obc_status.temp_eps = pkt->data[5];
            obc_status.temp_batt = pkt->data[6];
        } else if(pkt->app_id == COMMS) {
            obc_status.temp_comms = pkt->data[1];
        }
    }

    return R_OK;
}

OBC_returnStateTypedef hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t sid) {

    C_ASSERT(app_id < LAST_APP_ID) { return R_ERROR; }

    pkt->ser_subtype = TC_HK_REPORT_PARAMETERS;

    crt_pkt(pkt, app_id, TC, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TC_LD_ACK_UPLINK, OBC);

    pkt->data[0] = sid;
    pkt->len = 1;

    return R_OK;
}

OBC_returnStateTypedef hk_crt_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t sid) {

    pkt->data[0] = sid;

    if(sid == 3) {

        //cnv.cnv32 = time.now();
        cnv32_8(time.now(), pkt->data[1]);
        pkt->len = 5;
    } else if(sid == 4) {

        pkt->data[1] = obc_status.mode;
        pkt->data[2] = obc_status.batt_curr;
        pkt->data[3] = obc_status.batt_volt;
        pkt->data[4] = obc_status.bus_3v3_curr;
        pkt->data[5] = obc_status.bus_5v_curr;
        pkt->data[6] = obc_status.temp_eps;
        pkt->data[7] = obc_status.temp_batt;
        pkt->data[8] = obc_status.temp_comms;
        pkt->len = 9;
    }

    crt_pkt(pkt, OBC, TC, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TC_HK_PARAMETERS_REPORT, app_id);

    return R_OK;
}
