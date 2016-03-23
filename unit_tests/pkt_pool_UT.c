#include <stdio.h>

#include "../pkt_pool.h"
#include "../tc_tm.h"

#define TEST_ARRAY 20

int main() {

	SAT_returnState res;
	struct tc_tm_pkt *temp[MAX_POOL_PKT + 1];

	for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
		temp[i]=get_pkt();
		if(temp[i] == NULL) {
			printf("ERROR in get %d\n",i);
		} else {
			printf("OK in get %d\n",i);
		}
	}

	temp[MAX_POOL_PKT] = get_pkt();
	if(temp[MAX_POOL_PKT] != NULL) {
		printf("ERROR in last get\n");
	} else {
		printf("OK in last get\n");
	}

	for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
		res = free_pkt(temp[i]);
		if(res != SATR_OK) {
			printf("ERROR in free %d\n",i);
		} else {
			printf("OK in free %d\n",i);
		}
	}

	res = free_pkt(temp[MAX_POOL_PKT]);
	if(res != SATR_ERROR) {
		printf("ERROR in last get\n");
	} else {
		printf("OK in last get\n");
	}

    return 0;
}