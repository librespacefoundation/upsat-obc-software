#include "route_verification.h"

OBC_returnStateTypedef route_pkt(tc_tm_pkt *pkt) {
    uint16_t id;

    if(pkt->type == TC) {
        id = pkt->app_id;
    } else if(pkt->type == TM) {
        id = pkt->dest_id;
    } else {
        return R_ERROR;
    }

    if(id == OBC && pkt->ser_type == TC_HOUSEKEEPING_SERVICE && (pkt->ser_subtype == 21 || pkt->ser_subtype == 23)) {
        hk_app(pkt);
        return R_OK;
    } else if(id == OBC && pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE && pkt->ser_subtype == 1) {
        uint8_t fid = pkt->data[0];
        if(fid == 1 || fid == 2 || fid == 3 ){
            power_control_app(pkt);
            return R_OK;
        } else {
            return R_ERROR;
        }
    } else if(id == OBC && pkt->ser_type == TC_LARGE_DATA_SERVICE) {
        large_data_app(pkt);
        return R_OK;
    } else if(id == OBC && pkt->ser_type == TC_MASS_STORAGE_SERVICE && (pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13)) {
        mass_storage_app(pkt);
        return R_OK;
    } else if(id == EPS && pkt->ser_type == TC_HOUSEKEEPING_SERVICE && pkt->ser_subtype == 21) {
        hk_app(pkt);
        return R_OK;
    } else if(id == ADCS) {

    } else if(id == COMMS && pkt->ser_type == TC_HOUSEKEEPING_SERVICE && pkt->ser_subtype == 21) {
        hk_app(pkt);
        return R_OK;
    } else if(id == IAC) {

    } else if(id == GND && pkt->ser_type == TC_HOUSEKEEPING_SERVICE && pkt->ser_subtype == 25) {
        hk_app(pkt);
        return R_OK;
    } else {
        return R_ERROR;
    }

    return R_ERROR;
}
