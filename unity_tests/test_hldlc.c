#include "unity.h"
#include "hldlc.h"
#include "Mockservices.h"

#define TEST_ARRAY 20



void test_hldlc_frame1(void) {
    uint8_t check;
    uint16_t size = 10;
    uint16_t res_size = 12;
    uint8_t in[TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x22, 0x23};
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = {0x7E, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	                         0x22, 0x23, 0x7E};

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE(check == SATR_EOT);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(res, out, res_size);
}

void test_hldlc_frame2(void) {
    uint8_t check;
    uint16_t size = 10;
    uint16_t res_size = 14;
    uint8_t in[TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x7E, 0x16, 0x17, 0x18, 0x7D, 0x22, 0x23};
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = {0x7E, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                         0x7D, 0x5D, 0x22, 0x23, 0x7E };

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE(check == SATR_EOT);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(res, out, res_size);
}

void test_hldlc_deframe1(void) {
    uint8_t check;
    uint16_t size = 12;
    uint16_t res_size = 10, cnt_in = 0, cnt_out = 0;
    uint8_t in[TEST_ARRAY] = {0x7E, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	                      0x22, 0x23, 0x7E};
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x22, 0x23};

    check = HLDLC_deframe(in, out, &size);

    TEST_ASSERT_TRUE(check == SATR_EOT);
    TEST_ASSERT_TRUE(res_size == size);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(res, out, res_size);
}
void test_hldlc_deframe2(void) {
    uint8_t check;
    uint16_t size = 14;
    uint16_t res_size = 10, cnt_in = 0, cnt_out = 0;
    uint8_t in[TEST_ARRAY] =  {0x7E, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                       0x7D, 0x5D, 0x22, 0x23, 0x7E };
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x7E, 0x16, 0x17, 0x18, 0x7D, 0x22, 0x23};

    check = HLDLC_deframe(in, out, &size);

    TEST_ASSERT_TRUE(check == SATR_EOT);
    TEST_ASSERT_TRUE(res_size == size);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(res, out, res_size);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hldlc_frame1);
    RUN_TEST(test_hldlc_frame2);
    RUN_TEST(test_hldlc_deframe1);
    RUN_TEST(test_hldlc_deframe2);
    return UNITY_END();
}
