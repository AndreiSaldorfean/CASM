#include <stdio.h>
#include "ts_dict.h"
#include "casm_dict.h"
#include "unity.h"

void setUp(void) {
    casmInitDict();
}

void tearDown(void) {
    casmDestroyDict();
}

void IT_testAllCodes(void) {
    for(int i=0;i<CASM_DICT_SIZE;i++){
        TEST_ASSERT_NOT_EQUAL(kh_end(h),casmGetDict(gInstructionsTable[i].name));
    }
}

