#include "housekeeping_service.h"

struct _obc_status obc_status;

void hk_SCH() {

    tc_tm_pkt pkt;
  
    hk_crt_pkt_TC(&pkt, EPS_APP_ID, HEALTH_REP);
    route_pkt(&pkt);
    hk_crt_pkt_TC(&pkt, COMMS_APP_ID, HEALTH_REP);
    route_pkt(&pkt);
    //delay(59) sec;
    hk_crt_pkt_TM(&pkt, GND_APP_ID, WOD_REP);
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

SAT_returnState hk_app(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    if(pkt->ser_subtype == TC_HK_REPORT_PARAMETERS) {

        tc_tm_pkt *temp_pkt = 0;
        HK_struct_id sid = (HK_struct_id)pkt->data[0];

        hk_crt_free_pkt_TM(temp_pkt, (TC_TM_app_id)pkt->dest_id, sid);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(pkt);
    } else if(pkt->app_id == EPS_APP_ID && pkt->ser_subtype == TC_HK_PARAMETERS_REPORT) {
        obc_status.batt_curr = pkt->data[1];
        obc_status.batt_volt = pkt->data[2];
        obc_status.bus_3v3_curr = pkt->data[3];
        obc_status.bus_5v_curr = pkt->data[4];
        obc_status.temp_eps = pkt->data[5];
        obc_status.temp_batt = pkt->data[6];
    } else if(pkt->app_id == COMMS_APP_ID && pkt->ser_subtype == TC_HK_PARAMETERS_REPORT) {
        obc_status.temp_comms = pkt->data[1];
    }

    return SATR_OK;
}

hk_crt_free_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    pkt = get_pkt(NORMAL);
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    hk_crt_pkt_TM(pkt, app_id, sid);
    return SATR_OK;
}

SAT_returnState hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    if(!C_ASSERT(app_id < LAST_APP_ID) == true)  { return SATR_ERROR; }

    pkt->ser_subtype = TC_HK_REPORT_PARAMETERS;

    crt_pkt(pkt, app_id, TC, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TC_LD_ACK_UPLINK, OBC_APP_ID);

    pkt->data[0] = (char*)sid;
    pkt->len = 1;

    return SATR_OK;
}

SAT_returnState hk_crt_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    pkt->data[0] = (HK_struct_id)sid;

    if(sid == EX_HEALTH_REP) {

        //cnv.cnv32 = time.now();
        cnv32_8(time_now(), &pkt->data[1]);
        pkt->len = 5;
    } else if(sid == WOD_REP) {

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

    crt_pkt(pkt, OBC_APP_ID, TC, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TC_HK_PARAMETERS_REPORT, app_id);

    return SATR_OK;
}
