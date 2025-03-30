#include "unity.h"
#include "ts_dict.h"
#include "ts_encoder.h"
#include "ts_preprocessor.h"
#include "casm_dict.h"

void setUp(void) {
    casmInitInstructionTable();
}

void tearDown(void) {
    casmDestroyDict();
}


int main(){
    UNITY_BEGIN();
    RUN_TEST(TS_Dict0);
    RUN_TEST(TS_Encoder0);
    RUN_TEST(TS_Preprocessor0);
    RUN_TEST(TS_Preprocessor1);
    RUN_TEST(TS_Preprocessor2);
    RUN_TEST(TS_Preprocessor3);
    RUN_TEST(TS_Preprocessor4);
    RUN_TEST(TS_Preprocessor5);
    return UNITY_END();
}