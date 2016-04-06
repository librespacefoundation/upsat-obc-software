#include "unity.h"
#include "hldlc.h"
#include "Mockservices.h"

#include <stdio.h>

#define TEST_ARRAY 20


uint8_t in[TEST_ARRAY], out[TEST_ARRAY*2], res[TEST_ARRAY*2];
uint16_t cnt_in, cnt_out, size, res_size;
uint8_t c;

/* This function is run before each test. Here we
   can initialize any variables needed for the test */
void setup(void) {

    //Size of the input buffer
    size = 10;

}

void test_hldlc_frame1(void) {
    uint8_t check;
    uint16_t res_size = 12;
    size = 10;

    //Initilize the input array for the test
    in[0] = 0x12;
    in[1] = 0x13;
    in[2] = 0x14;
    in[3] = 0x15;
    in[4] = 0x16;
    in[5] = 0x17;
    in[6] = 0x18;
    in[7] = 0x19;
    in[8] = 0x22;
    in[9] = 0x23;

    //Initialize the result array for the test.
    //We will compare this with the actual result
    //of the function.
    res[0] = 0x7E;
    res[1] = 0x12;
    res[2] = 0x13;
    res[3] = 0x14;
    res[4] = 0x15;
    res[5] = 0x16;
    res[6] = 0x17;
    res[7] = 0x18;
    res[8] = 0x19;
    res[9] = 0x22;
    res[10] = 0x23;
    res[11] = 0x7E;


    check = HLDLC_frame(in, out, &size);

    TEST_ASSERT_TRUE(check == SATR_EOT);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(res, out, res_size);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hldlc_frame1);
    return UNITY_END();
}
