#include "housekeeping.h"

#undef __FILE_ID__
#define __FILE_ID__ 666

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