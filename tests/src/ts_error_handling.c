#include "ts_error_handling.h"
#include "unity.h"
#include "casm_preprocessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "casm_encoder.h"
#include "casm_utils.h"
#include "casm_dict.h"

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

/* ============================================================================================================
                                            TYPEDEFS AND STRUCTS
============================================================================================================ */

/* ============================================================================================================
                                            Local Variables
============================================================================================================ */
static casm_program_t program[200] =
{
    {
        .instruction = "mov r111,r0",
        .address = 0
    },
    {
        .instruction = "mov [r20],[r2]",
        .address = 2
    },
    {
        .instruction = "ret",
        .address = 4
    },
    {
        .instruction = "jmp 4",
        .address = 6
    },
    {
        .instruction = "ret",
        .address = 8
    },
    {
        .instruction = "jmp -10",
        .address = 10
    },
    {
        .instruction = "ret",
        .address = 14
    },
};
/* ============================================================================================================
                                            Global Variables
============================================================================================================ */

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

/* ============================================================================================================
                                            Global functions
============================================================================================================ */
void TS_ERROR_HANDLING1(void)
{
    casmInitInstructionTable();
    casmInitRegTable();

    for(int i = 0; program[i].instruction[0]; i++)
    {
        int type = getInstructionType(program[i].instruction);
        if(type == CASM_ERR_MASK)
        {
            printf("error\n");
        }
        casmInstructionFrame_t instr = encodeInstruction(program[i].instruction, type);
        if(instr.instr == 0xFFFF)
        {
            printf("error: %s\n",program[i].instruction);
        }

    }

    kh_destroy(INST_TABLE, hIns);
    kh_destroy(REG_TABLE, hReg);
}