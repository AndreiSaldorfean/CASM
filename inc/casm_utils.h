#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

#define START_ADDR    0x0000U
#define CASM_B2_MASK  (0xA000U)
#define CASM_B3_MASK  (0xC000U)
#define CASM_B4_MASK  (0xE000U)
#define CASM_ERR_MASK (0xF000U)
#define CASM_STOP     (0xF001U)

#define CASM_OPPCODE_RGX ("\\w+\\D")
#define CASM_OPERAND1_RGX ("\\S+,")
#define CASM_OPERAND2_RGX (",+.+\\s")
#define CASM_SINGLE_OPERAND_RGX (" +.+\\s")

#define CASM_IMM_AM_RGX ("[^Rr]+\\d+$")
#define CASM_DIRECT_AM_RGX ("\\[[Rr]")
#define CASM_INDIRECT_AM_RGX ("^\\[+.+\\]+$")
#define CASM_INDEXED_AM_RGX ("\\d+\\[+.+\\]+$")
#define CASM_HELP_MESSAGE ("Usage: casm [OPTIONS]... [path/to/input/file] [-o] [path/to/ouput/file] \n\nOptions:\n  -h\t\t\tDisplay this help message\n  -o\t\t\tOutputs the binary to an specified file\n  -v\t\t\tEnables verbose mode\n  -S\t\t\tPrint symbol table\n")

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
void get_binary_num(uint16_t num, char* result);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */
