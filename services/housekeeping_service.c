#include "housekeeping_service.h"


#undef __FILE_ID__
#define __FILE_ID__ 5

tc_tm_pkt hk_pkt;
uint8_t hk_pkt_data[MAX_PKT_DATA];

void hk_INIT() {
   hk_pkt.data = hk_pkt_data;
}

void hk_SCH() {
  
    hk_crt_pkt_TC(&hk_pkt, EPS_APP_ID, HEALTH_REP);
    route_pkt(&hk_pkt);
    hk_crt_pkt_TC(&hk_pkt, COMMS_APP_ID, HEALTH_REP);
    route_pkt(&hk_pkt);
    HAL_obc_delay(59000);
    hk_crt_pkt_TM(&hk_pkt, GND_APP_ID, WOD_REP);
    route_pkt(&hk_pkt);
    clear_wod();
}

void clear_wod() {
        sat_status.batt_curr = 0;
        sat_status.batt_volt = 0;
        sat_status.bus_3v3_curr = 0;
        sat_status.bus_5v_curr = 0;
        sat_status.temp_eps = 0;
        sat_status.temp_batt = 0;
        sat_status.temp_comms = 0;
}

SAT_returnState hk_app(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == TC_HK_REPORT_PARAMETERS || pkt->ser_subtype == TM_HK_PARAMETERS_REPORT) == true) { return SATR_ERROR; }

    if(pkt->ser_subtype == TC_HK_REPORT_PARAMETERS) {

        tc_tm_pkt *temp_pkt = 0;
        HK_struct_id sid = (HK_struct_id)pkt->data[0];

        hk_crt_empty_pkt_TM(&temp_pkt, (TC_TM_app_id)pkt->dest_id, sid);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);

    } else if(pkt->app_id == EPS_APP_ID && pkt->ser_subtype == TM_HK_PARAMETERS_REPORT) {
        sat_status.batt_curr = pkt->data[1];
        sat_status.batt_volt = pkt->data[2];
        sat_status.bus_3v3_curr = pkt->data[3];
        sat_status.bus_5v_curr = pkt->data[4];
        sat_status.temp_eps = pkt->data[5];
        sat_status.temp_batt = pkt->data[6];

        pkt->verification_state = SATR_OK;

    } else if(pkt->app_id == COMMS_APP_ID && pkt->ser_subtype == TM_HK_PARAMETERS_REPORT) {
        sat_status.temp_comms = pkt->data[1];

        pkt->verification_state = SATR_OK;
    }

    return SATR_OK;
}

SAT_returnState hk_crt_empty_pkt_TM(tc_tm_pkt **pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    *pkt = get_pkt();
    if(!C_ASSERT(*pkt != NULL) == true) { return SATR_ERROR; }

    hk_crt_pkt_TM(*pkt, app_id, sid);
    return SATR_OK;
}

SAT_returnState hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    if(!C_ASSERT(app_id < LAST_APP_ID) == true)  { return SATR_ERROR; }

    crt_pkt(pkt, app_id, TC, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TC_HK_REPORT_PARAMETERS, SYSTEM_APP_ID);

    pkt->data[0] = (char)sid;
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

        pkt->data[1] = sat_status.mode;
        pkt->data[2] = sat_status.batt_curr;
        pkt->data[3] = sat_status.batt_volt;
        pkt->data[4] = sat_status.bus_3v3_curr;
        pkt->data[5] = sat_status.bus_5v_curr;
        pkt->data[6] = sat_status.temp_eps;
        pkt->data[7] = sat_status.temp_batt;
        pkt->data[8] = sat_status.temp_comms;
        pkt->len = 9;
    }

    crt_pkt(pkt, SYSTEM_APP_ID, TM, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TM_HK_PARAMETERS_REPORT, app_id);

    return SATR_OK;
}
