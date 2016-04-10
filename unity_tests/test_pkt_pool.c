#include "unity.h"
#include "pkt_pool.h"
#include "Mockservices.h"
#include <stdio.h>

void test_pkt_pool_init_get_free(void) {
    SAT_returnState res;
    tc_tm_pkt *temp[POOL_PKT_SIZE + 1];

    res = pkt_pool_INIT();
    TEST_ASSERT_TRUE_MESSAGE(res == SATR_OK, "Initializaiton failed");



    for(uint8_t i = 0; i < POOL_PKT_SIZE;  i++) {
	time_now_ExpectAndReturn(0);
	temp[i] = get_pkt();
	TEST_ASSERT_TRUE_MESSAGE(temp[i] != NULL, "one of the packets was null");
    }

    temp[POOL_PKT_SIZE] = get_pkt();
    TEST_ASSERT_TRUE_MESSAGE(temp[POOL_PKT_SIZE] == NULL, "the last get_pkt() call returned non null");

    for(uint8_t i = 0; i < POOL_PKT_SIZE; i++) {
	res = free_pkt(temp[i]);
	TEST_ASSERT_TRUE_MESSAGE(res == SATR_OK, "free_pkt returned error");
    }

    tst_debugging_IgnoreAndReturn(false);
    res = free_pkt(temp[POOL_PKT_SIZE]);
    TEST_ASSERT_TRUE_MESSAGE(res == SATR_ERROR, "free_pkt on null packet didnt return error");

}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_pkt_pool_init_get_free);

    return UNITY_END();
}
