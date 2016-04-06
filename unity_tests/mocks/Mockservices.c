/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockservices.h"

static const char* CMockString_e = "e";
static const char* CMockString_f = "f";
static const char* CMockString_fi = "fi";
static const char* CMockString_l = "l";
static const char* CMockString_time_now = "time_now";
static const char* CMockString_tst_debugging = "tst_debugging";

typedef struct _CMOCK_time_now_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint32_t ReturnVal;

} CMOCK_time_now_CALL_INSTANCE;

typedef struct _CMOCK_tst_debugging_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  uint8_t ReturnVal;
  char* Expected_f;
  int Expected_fi;
  int Expected_l;
  char* Expected_e;

} CMOCK_tst_debugging_CALL_INSTANCE;

static struct MockservicesInstance
{
  CMOCK_MEM_INDEX_TYPE time_now_CallInstance;
  CMOCK_MEM_INDEX_TYPE tst_debugging_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockservices_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_SET_DETAIL(CMockString_time_now);
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.time_now_CallInstance, cmock_line, CMockStringCalledLess);
  UNITY_SET_DETAIL(CMockString_tst_debugging);
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.tst_debugging_CallInstance, cmock_line, CMockStringCalledLess);
}

void Mockservices_Init(void)
{
  Mockservices_Destroy();
}

void Mockservices_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

uint32_t time_now(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_SET_DETAIL(CMockString_time_now);
  CMOCK_time_now_CALL_INSTANCE* cmock_call_instance = (CMOCK_time_now_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.time_now_CallInstance);
  Mock.time_now_CallInstance = CMock_Guts_MemNext(Mock.time_now_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void time_now_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_time_now_CALL_INSTANCE));
  CMOCK_time_now_CALL_INSTANCE* cmock_call_instance = (CMOCK_time_now_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.time_now_CallInstance = CMock_Guts_MemChain(Mock.time_now_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  UNITY_CLR_DETAILS();
}

uint8_t tst_debugging(char* f, int fi, int l, char* e)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_SET_DETAIL(CMockString_tst_debugging);
  CMOCK_tst_debugging_CALL_INSTANCE* cmock_call_instance = (CMOCK_tst_debugging_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.tst_debugging_CallInstance);
  Mock.tst_debugging_CallInstance = CMock_Guts_MemNext(Mock.tst_debugging_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_SET_DETAILS(CMockString_tst_debugging,CMockString_f);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_f, f, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_tst_debugging,CMockString_fi);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_fi, fi, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_tst_debugging,CMockString_l);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_l, l, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_tst_debugging,CMockString_e);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_e, e, cmock_line, CMockStringMismatch);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_tst_debugging(CMOCK_tst_debugging_CALL_INSTANCE* cmock_call_instance, char* f, int fi, int l, char* e)
{
  cmock_call_instance->Expected_f = f;
  cmock_call_instance->Expected_fi = fi;
  cmock_call_instance->Expected_l = l;
  cmock_call_instance->Expected_e = e;
}

void tst_debugging_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char* f, int fi, int l, char* e, uint8_t cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_tst_debugging_CALL_INSTANCE));
  CMOCK_tst_debugging_CALL_INSTANCE* cmock_call_instance = (CMOCK_tst_debugging_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.tst_debugging_CallInstance = CMock_Guts_MemChain(Mock.tst_debugging_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_tst_debugging(cmock_call_instance, f, fi, l, e);
  cmock_call_instance->ReturnVal = cmock_to_return;
  UNITY_CLR_DETAILS();
}

