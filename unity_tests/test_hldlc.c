#include "unity.h"
#include "hldlc.h"
#include "Mockservices.h"
#include <stdio.h>

#define TEST_ARRAY 20

/*
* =========
* | frame |
* =========
*/

void test_hldlc_frame_without_escape_octet(void) {
    uint8_t check;
    uint16_t size = 10;
    uint16_t res_size = 12;
    uint8_t in[TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x22, 0x23};
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = {0x7E, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	                         0x22, 0x23, 0x7E};

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_EOT, "Function didn't return SATR_EOT");

    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(res, out, res_size, "Output wasn't the expected one");
}

void test_hldlc_frame_with_escape_octet(void) {
    uint8_t check;
    uint16_t size = 10;
    uint16_t res_size = 14;
    uint8_t in[TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x7E, 0x16, 0x17, 0x18, 0x7D, 0x22, 0x23};
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = {0x7E, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                         0x7D, 0x5D, 0x22, 0x23, 0x7E };

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_EOT, "Function didn't return SATR_EOT");

    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(res, out, res_size, "Output wasn't the expected one");
}

void test_hldlc_frame_null_input(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size = 10;
    uint8_t *in = NULL;
    uint8_t out[TEST_ARRAY*2];

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

void test_hldlc_frame_null_output(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size = 10;
    uint8_t in[TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x7E, 0x16, 0x17, 0x18, 0x7D, 0x22, 0x23};
    uint8_t *out = NULL;

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

void test_hldlc_frame_null_size(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size;
    uint8_t in[TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x7E, 0x16, 0x17, 0x18, 0x7D, 0x22, 0x23};
    uint8_t out[TEST_ARRAY*2];

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

/*
* ===========
* | deframe |
* ===========
*/

void test_hldlc_deframe_without_escape_octet(void) {
    uint8_t check;
    uint16_t size = 12;
    uint16_t res_size = 10, cnt_in = 0, cnt_out = 0;
    uint8_t in[TEST_ARRAY] = {0x7E, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	                      0x22, 0x23, 0x7E};
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x22, 0x23};

    check = HLDLC_deframe(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_EOT, "Function didn't return SATR_EOT");
    TEST_ASSERT_TRUE_MESSAGE(res_size == size, "Output size was different than the expected");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(res, out, res_size, "Output wasn't the expected one");
}

void test_hldlc_deframe_with_escape_octet(void) {
    uint8_t check;
    uint16_t size = 14;
    uint16_t res_size = 10, cnt_in = 0, cnt_out = 0;
    uint8_t in[TEST_ARRAY] =  {0x7E, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                       0x7D, 0x5D, 0x22, 0x23, 0x7E };
    uint8_t out[TEST_ARRAY*2];
    uint8_t res[2*TEST_ARRAY] = { 0x12, 0x13, 0x14, 0x7E, 0x16, 0x17, 0x18, 0x7D, 0x22, 0x23};

    check = HLDLC_deframe(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_EOT, "Function didn't return SATR_EOT");
    TEST_ASSERT_TRUE_MESSAGE(res_size == size, "Output size was different than the expected");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(res, out, res_size, "Output wasn't the expected one");
}

void test_hldlc_deframe_null_input(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size = 10;
    uint8_t *in = NULL;
    uint8_t out[TEST_ARRAY*2];

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

void test_hldlc_deframe_null_output(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size = 10;
    uint8_t in[TEST_ARRAY] =  {0x7E, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                       0x7D, 0x5D, 0x22, 0x23, 0x7E };
    uint8_t *out = NULL;

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

void test_hldlc_deframe_non_start_flag_as_first_char(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size = 10;
    uint8_t in[TEST_ARRAY] =  {0x12, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                       0x7D, 0x5D, 0x22, 0x23, 0x7E };
    uint8_t *out = NULL;

    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

void test_hldlc_deframe_packet_size_bigger_than_maximum(void) {
    //Ignore debugging function
    tst_debugging_IgnoreAndReturn(false);

    uint8_t check;
    uint16_t size = TC_MAX_PKT_SIZE + 1;
    uint8_t in[TEST_ARRAY] =  {0x7E, 0x12, 0x13, 0x14, 0x7D, 0x5E, 0x16, 0x17, 0x18,
	                       0x7D, 0x5D, 0x22, 0x23, 0x7E };
    uint8_t out[TEST_ARRAY*2];

    check = HLDLC_deframe(in, out, &size);
    TEST_ASSERT_TRUE_MESSAGE(check == SATR_ERROR, "Function didn't return SATR_ERROR");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_hldlc_frame_without_escape_octet);
    RUN_TEST(test_hldlc_frame_with_escape_octet);
    RUN_TEST(test_hldlc_frame_null_input);
    RUN_TEST(test_hldlc_frame_null_output);
    RUN_TEST(test_hldlc_frame_null_size);

    RUN_TEST(test_hldlc_deframe_without_escape_octet);
    RUN_TEST(test_hldlc_deframe_with_escape_octet);
    RUN_TEST(test_hldlc_deframe_null_input);
    RUN_TEST(test_hldlc_deframe_null_output);
    RUN_TEST(test_hldlc_deframe_non_start_flag_as_first_char);
    RUN_TEST(test_hldlc_deframe_packet_size_bigger_than_maximum);

    return UNITY_END();
}
