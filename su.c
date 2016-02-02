

/*  
REQ: MNLP-013
*/
#define SU_TIMEOUT 400 

/* 
section 13.6
*/
#define SU_SCR_HDR_RES   0
#define SU_SCR_HDR_INMS  1
#define SU_SCR_HDR_MNLP  2
#define SU_SCR_HDR_FIPEX 3

/*
REQ: MNLP-027
*/
#define SU_SCR_TT_S1  0x41
#define SU_SCR_TT_S2  0x42
#define SU_SCR_TT_S3  0x43
#define SU_SCR_TT_S4  0x44
#define SU_SCR_TT_S5  0x45
#define SU_SCR_TT_EOR 0x55

/*
Section 13.10

when 0x00 is undefined
*/
#define OBC_SU_ON_CMD_ID    0xF1
#define OBC_SU_OFF_CMD_ID   0xF2
#define SU_RESET_CMD_ID     0x02
#define SU_LDP_CMD_ID       0x05
#define SU_HC_CMD_ID        0x06
#define SU_CAL_CMD_ID       0x07
#define SU_SCI_CMD_ID       0x08
#define SU_HK_CMD_ID        0x09
#define SU_STM_CMD_ID       0x0A
#define SU_DUMP_CMD_ID      0x0B
#define SU_BIAS_ON_CMD_ID   0x53
#define SU_BIAS_OFF_CMD_ID  0xC9
#define SU_MTEE_ON_CMD_ID   0x35
#define SU_MTEE_OFF_CMD_ID  0x9C
#define SU_ERR_CMD_ID       0x00
#define OBC_SU_ERR_CMD_ID   0x00
#define OBC_EOT_CMD_ID      0xFE

#define OBC_SU_ON_RSP_ID    0x00
#define OBC_SU_OFF_RSP_ID   0x00
#define SU_RESET_RSP_ID     0x00
#define SU_LDP_RSP_ID       0x05
#define SU_HC_RSP_ID        0x06
#define SU_CAL_RSP_ID       0x07
#define SU_SCI_RSP_ID       0x08
#define SU_HK_RSP_ID        0x09
#define SU_STM_RSP_ID       0x0A
#define SU_DUMP_RSP_ID      0x0B
#define SU_BIAS_ON_RSP_ID   0x00
#define SU_BIAS_OFF_RSP_ID  0x00
#define SU_MTEE_ON_RSP_ID   0x00
#define SU_MTEE_OFF_RSP_ID  0x00
#define SU_ERR_RSP_ID       0xBB
#define OBC_SU_ERR_RSP_ID   0xFA
#define OBC_EOT_RSP_ID      0x00

/*
REQ: MNLP-031
*/
#define SU_RSP_PCKT_DATA_SIZE  172
#define SU_RSP_PCKT_SIZE       174

struct OBC_data {
	uint32_t time_epoch;
	int16_t  roll;
	int16_t  pitch;
	int16_t  yaw;
	int16_t  roll_dot;
	int16_t  pitch_dot;
	int16_t  yaw_dot;
	uint16_t x_eci;
	uint16_t y_eci;
	uint16_t z_eci;
};

struct script_handler_data 
{
	uint16_t su_timeout
	uint8_t state;
	struct  su_script buffer[7];
};

struct su_script {
	script_hdr header;
	stack script_times_table time_table; 
	uint8_t c0;
	uint8_t c1;
};

/* 
section 13.6

time format?
ID_SWver: b4-b0 sw ver,b6-b5 su id 
MD_type:  b4-b0 script type, b6-b5 su model
*/
struct script_hdr 
{
	uint16_t script_len;
	uint32_t start_time;
	uint32_t file_sn;
	uint8_t  ID_SWver;
	uint8_t  MD_type;
};

/* 
REQ: MNLP-027

*/
struct script_times_table
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t index;
	stack script_seq seq;
};



/* 
REQ: MNLP-027
parameters size is variable, thus the pointer.
*/
struct script_seq
{
	uint8_t dTIME_sec;
	uint8_t dTIME_min;
	uint8_t cmd_id;
	uint8_t len;
	uint8_t seq_cnt;
	uint8_t * parameters;
};


/*


*/
struct science_data {
	struct science_hdr header;
	struct response_pckt res_pck;
}; 

/*
REQ: MNLP-022

*/
struct science_hdr 
{
	uint32_t time_epoch;
	int16_t  roll;
	int16_t  pitch;
	int16_t  yaw;
	int16_t  roll_dot;
	int16_t  pitch_dot;
	int16_t  yaw_dot;
	uint16_t x_eci;
	uint16_t y_eci;
	uint16_t z_eci;
};

/*
REQ: MNLP-031

*/
struct response_pckt 
{
	uint8_t rsp_id;
	uint8_t seq_cnt;
	uint8_t data[SU_RSP_PCKT_DATA_SIZE];
};

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
