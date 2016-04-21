#include "housekeeping.h"

#undef __FILE_ID__
#define __FILE_ID__ 666

SAT_returnState hk_parameters_report(TC_TM_app_id app_id, HK_struct_id sid, uint8_t *data) {
   return SATR_ERROR;
}

uint8_t wod_test[6] = { 1,2,3,4,5,6 };

SAT_returnState hk_report_parameters(HK_struct_id sid, tc_tm_pkt *pkt) {
    
    pkt->data[0] = (HK_struct_id)sid;
    
    if(sid == HEALTH_REP) {
        pkt->data[1] = wod_test[0];
        pkt->data[2] = wod_test[1];
        pkt->data[3] = wod_test[2];
        pkt->data[4] = wod_test[3];
        pkt->data[5] = wod_test[4];
        pkt->data[6] = wod_test[5];

        wod_test[0] += 10;
        wod_test[1] += 10;
        wod_test[2] += 10;
        wod_test[3] += 10;
        wod_test[4] += 10;
        wod_test[5] += 10;

        pkt->len = 7;
    } else if(sid == EX_HEALTH_REP) {

        //cnv.cnv32 = time.now();
        cnv32_8(time_now(), &pkt->data[1]);
        pkt->len = 5;
    }

    return SATR_OK;
}