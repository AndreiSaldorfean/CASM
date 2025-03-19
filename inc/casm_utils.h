#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

#define CASM_B2_MASK (0x8000U)
#define CASM_B3_MASK (0xC000U)
#define CASM_B4_MASK (0xE000U)
#define CASM_ERR_MASK (0xF000U)

#define CASM_OPPCODE_RGX ("\\w+\\D")
#define CASM_OPERAND1_RGX ("\\S+,")
#define CASM_OPERAND2_RGX (",+.+$")
#define CASM_SINGLE_OPERAND_RGX (" +.+$")

#define CASM_IMM_AM_RGX ("[^R]+\\d+$")
#define CASM_DIRECT_AM_RGX ("\\(R")
#define CASM_INDIRECT_AM_RGX ("^\\(+.+\\)+$")
#define CASM_INDEXED_AM_RGX ("\\d+\\(+.+\\)+$")
#define CASM_HELP_MESSAGE ("Usage: casm [-h] [path/to/input/file] [-o] [path/to/ouput/file] \n\nOptions:\n  -h\t\t\tDisplay this help message\n  -o\t\t\tOutputs the binary to an specified file")

/* ============================================================================================================
                                            TYPEDEFS AND STRUCTS
============================================================================================================ */

typedef struct
{
    uint16_t instr;
    uint16_t offset1;
    uint16_t offset2;
}casmInstructionFrame_t;

/* ============================================================================================================
                                            Local Variables
============================================================================================================ */

/* ============================================================================================================
                                            Global Variables
============================================================================================================ */

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

void trim_whitespace(char *str);
void print_binary(uint16_t num);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */
