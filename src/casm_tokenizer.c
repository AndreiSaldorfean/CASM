#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "re.h"
#include "stdlib.h"
#include "casm_utils.h"

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

/* ============================================================================================================
                                            TYPEDEFS AND STRUCTS
============================================================================================================ */

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

void tokenizeInstructionTypeB1(const char* inst, char** destination) {
    int match_len = 0;

    int offset = re_match(CASM_OPPCODE_RGX, inst,&match_len);
    memcpy(destination[0], inst + offset, match_len);
    destination[0][match_len] = '\0';
    trim_whitespace(destination[0]);

    offset = re_match(CASM_OPERAND1_RGX, inst,&match_len);
    memcpy(destination[1], inst + offset, match_len);
    trim_whitespace(destination[1]);
    destination[1][match_len-1] = '\0';

    offset = re_match(CASM_OPERAND2_RGX, inst,&match_len);
    memcpy(destination[2], inst + offset+1, match_len);
    trim_whitespace(destination[2]);
    destination[2][match_len] = '\0';
}

void tokenizeInstructionTypeB2(const char* inst, char** destination) {
    int match_len = 0;

    int offset = re_match(CASM_OPPCODE_RGX, inst,&match_len);
    memcpy(destination[0], inst + offset, match_len);
    destination[0][match_len] = '\0';
    trim_whitespace(destination[0]);

    offset = re_match(CASM_SINGLE_OPERAND_RGX, inst,&match_len);
    memcpy(destination[1], inst + offset, match_len);
    trim_whitespace(destination[1]);
}

void tokenizeInstructionTypeB3(const char* inst, char** destination) {
    int match_len = 0;

    int offset = re_match(CASM_OPPCODE_RGX, inst,&match_len);
    memcpy(destination[0], inst + offset, match_len);
    destination[0][match_len] = '\0';
    trim_whitespace(destination[0]);

    offset = re_match(CASM_SINGLE_OPERAND_RGX, inst,&match_len);
    memcpy(destination[1], inst + offset, match_len);
    trim_whitespace(destination[1]);
}

void tokenizeInstructionTypeB4(const char* inst, char** destination) {
    int match_len = 0;

    int offset = re_match(CASM_OPPCODE_RGX, inst,&match_len);
    memcpy(destination[0], inst + offset, match_len);
    destination[0][match_len] = '\0';
    trim_whitespace(destination[0]);
}
