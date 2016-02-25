#include "power_ctrl.h"

uint8_t power_control_app(struct tc_tm_pkt *pkt) {
	uint8_t res, did, fid;

	did = pkt->data[0];
	fid = pkt->data[4];
	res = power_control_app_api( did, fid);
	verify_pkt(pkt, TC_ACK_ACC, res);
	return R_OK;
}

/*Must use real pins*/
uint8_t power_control_app_api( uint8_t did, uint8_t fid) {

	if(did == SD1 && fid == TURN_ON) {
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		return R_OK; 
	} else if(did == SD1 && fid == TURN_OFF) {
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		return R_OK;
	} else if(did == SD1 && fid == RESET) {
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		//delay
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		return R_OK;
	} else if(did == OBC && fid == RESET) {
		return R_OK;
	} else { 
		return R_ERROR; 
	}
}