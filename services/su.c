#include "su.h"

/*
This function is called every sec
*/
void su_script_handler() {
	if(state == idle || state == running) {
		for(int i = 0; i<SCRIPTS; i++) {
			if(scripts[i].start_time >= OBC_data.time_epoch && scripts[i].start_time != 0) {
				state = running
				running_script = i;
				
				do {
					struct script_times_table tt_temp;
					stack_peek(&scripts[i].time_table, &tt_temp);

					if( tt_temp.time == OBC_data.time_epoch) {
						stack_pop(&scripts[i].time_table, &tt_temp);
						seq_to_run = tt_temp.script_seq;
						state = running_seq;
						post_ev(run_seq);
						next_flag = 1;
						break;
					} else if( tt_temp.time < OBC_data.time_epoch) {
						stack_pop(&scripts[i].time_table, &tt_temp);
					} else if( tt_temp.time > OBC_data.time_epoch) {
						break;
					}
				} while(1);

				if(next_flag == 1) {
					break;
				}

			}
		}
	}
}

void su_run_seq() {

	do {

		post_ev(run_cmd);
	 	delay(seq.delay);
	 	seq.next();
	} while(seq.cmd_id !=  OBC_EOT_CMD_ID);
	state = idle;
}

void su_run_cmd() {
	if(cmd_id == OBC_SU_ON_CMD_ID) {
		eps_su_mnlp_pwr(ON);
	} else if(cmd_id == OBC_SU_OFF_CMD_ID) {
		eps_su_mnlp_pwr(OFF);
	} else if(cmd_id == OBC_SU_OFF_CMD_ID) {

	} else {
		uint8_t su_cmd[len];
		su_cmd[0] = cmd_id;

	}
}


/*
draft implementation
*/
void su_err_handler(uint8_t error) {
	if(error == timeout) {
		struct su_mnlp_obc_su_err_pckt err_pckt;
		eps_su_mnlp_pwr(OFF);
		generate_OBC_SU_ERR_pckt(err_pckt);
		delay_sec(60);
		sd_store_pckt(err_pckt);
		eps_su_mnlp_pwr(ON);

	} else if (error == su_err_pckt) {
		eps_su_mnlp_pwr(OFF);
		delay_sec(60);
		eps_su_mnlp_pwr(ON);
	} else {
		ERROR
	}
}

struct science_data pck;
struct OBC_data flight_data;
uint8_t su_rx_buf[SU_RSP_PCKT_SIZE];

void su_rx_callback(char c) {
	static cnt = 0;
	
	if(cnt<SU_RSP_PCKT_SIZE) {
		buf[c] = c;
		cnt++;
	} else {
		reset_su_timeout_timer();
		cnt = 0;
		pck.res_pck.rsp_id = buf[0];
		pck.res_pck.seq_cnt = buf[1]
		for(int i = 0; i < SU_RSP_PCKT_DATA_SIZE; i++) {
			pck.res_pck.data[i] = buf[i+2];
		}
		post_ev(handle_packet);
	}
}

void handle_packet(){
	
	if( pck.res_pck.rsp_id == SU_ERR_RSP_ID) {
		post_ev(su_err_pckt);
	}

	fill_science_header();
	post_ev(sd_log_su_pck);

}

void fill_science_header() {
	pck.header.time_epoch = flight_data.time_epoch ;
	pck.header.roll = flight_data.roll ;
	pck.header.pitch = flight_data.pitch ;
	pck.header.yaw = flight_data.yaw ;
	pck.header.roll_dot = flight_data.roll_dot ;
	pck.header.pitch_dot = flight_data.pitch_dot ;
	pck.header.yaw_dot = flight_data.yaw_dot ;
	pck.header.x_eci = flight_data.x_eci ;
	pck.header.y_eci = flight_data.y_eci ;
	pck.header.z_eci = flight_data.z_eci ;

}
