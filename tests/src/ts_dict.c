#include "unity.h"
#include "casm_dict.h"
#include <stdio.h>
#include "ts_dict.h"
#include "casm_inst_formats.h"

void TS_Dict0(void) {
    casmInitInstructionTable();

    for(int i=0;i<CASM_DICT_SIZE;i++){
        int var = csamGetInstruction(gInstructionsTable[i].name);
        TEST_ASSERT_NOT_EQUAL(kh_end(hIns),var);
    }

    casmDestroyDict();
}
