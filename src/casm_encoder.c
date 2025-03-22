#include "casm_encoder.h"
#include <stdint.h>
#include "casm_tokenizer.h"
#include "casm_utils.h"
#include "re.h"
#include <stdio.h>
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

/* ============================================================================================================
                                            Global Variables
============================================================================================================ */
int gIndex = 0;
/* ============================================================================================================
                                            Local functions
============================================================================================================ */

static uint16_t getAddressingMode(const char* reg)
{
    int match_len = 0;
    int match = re_match(CASM_IMM_AM_RGX, reg, &match_len);
    if(match != -1){
        return 0;
    }

    match = re_match(CASM_DIRECT_AM_RGX, reg, &match_len);
    if(match == -1){
        return 1;
    }

    match = re_match(CASM_INDIRECT_AM_RGX, reg, &match_len);
    if(match != -1){
        return 2;
    }

    match = re_match(CASM_INDEXED_AM_RGX, reg, &match_len);
    if(match != -1){
        return 3;
    }

    return 0;
}

static casmInstructionFrame_t getEncodedInstruction(char** instructionTokens, uint16_t instrType)
{
    casmInstructionFrame_t frame = {0};
    int off = 0;
    int offset = 0;
    uint8_t reg = 0;

    switch (instrType)
    {
        case 0:
        {
            /*--------------------------- SOURCE ------------------------------*/
            uint8_t ams = getAddressingMode(instructionTokens[2]);
            if (ams == 3)
            {
                off = 0;
                offset = re_match("\\d+\\(",instructionTokens[2],&off);
                char buffer[4] = {0};
                memcpy(buffer, instructionTokens[2] + offset, off-1);

                long num = strtol(buffer, NULL, 10);
                frame.offset1 = num;
            }
            frame.instr |= (ams<<10);

            off = 0;
            offset = re_match("[Rr]",instructionTokens[2],&off);
            reg = *(instructionTokens[2]+offset+1) - '0';
            frame.instr |= (reg<<6);

            /*--------------------------- DESTINATION ------------------------------*/
            uint8_t amd = getAddressingMode(instructionTokens[1]);
            if (amd == 3 )
            {
                off = 0;
                offset = re_match("\\d+\\(",instructionTokens[1],&off);
                char buffer[4] = {0};
                memcpy(buffer, instructionTokens[1] + offset, off-1);

                long num = strtol(buffer, NULL, 10);
                frame.offset2 = num;
            }

            frame.instr |= (amd<<4);
            off = 0;
            offset = re_match("[Rr]",instructionTokens[1],&off);
            reg = *(instructionTokens[1]+offset+1) - '0';
            frame.instr |= reg;
            break;

        }

        case CASM_B2_MASK:
        {
            uint8_t amd = getAddressingMode(instructionTokens[1]);
            if (amd == 3 )
            {
                off = 0;
                offset = re_match("\\d+\\(",instructionTokens[1],&off);
                char buffer[4] = {0};
                memcpy(buffer, instructionTokens[1] + offset, off-1);

                long num = strtol(buffer, NULL, 10);
                frame.offset1 = num;
            }

            frame.instr |= (amd<<4);
            off = 0;
            offset = re_match("[Rr]",instructionTokens[1],&off);
            reg = *(instructionTokens[1]+offset+1) - '0';
            frame.instr |= reg;
            break;
        }

        case CASM_B3_MASK:
        {
            long num = strtol(instructionTokens[1], NULL, 10);
            frame.instr |= (num);
            break;
        }

        case CASM_B4_MASK:
        {
            break;
        }

        default:
            printf("ERROR\n");
            break;
    }

    uint16_t opcode = casmGetDict(instructionTokens[0]);
    frame.instr |= opcode;

    return frame;
}

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

/**
 * @brief  Looks at the last 3 bits. Possible types: B1, B2, B3, B4
 * @param  inst: The instruction string
 * @retval The type of the instruction
 */
uint16_t getInstructionType(char* inst)
{
    int size = 0;
    int match = re_match(CASM_OPPCODE_RGX, inst, &size);
    char buffer[7] = {0};

    if(inst[0] == '\n' || inst[0] == '\0')
    {
        return CASM_STOP;
    }

    memcpy(buffer, inst + match, size);
    buffer[size] = '\0';

    trim_whitespace(buffer);

    switch (casmGetDict(buffer) & CASM_ERR_MASK)
    {
        case CASM_B2_MASK:
            return CASM_B2_MASK;
        case CASM_B3_MASK:
            return CASM_B3_MASK;
        case CASM_B4_MASK:
            return CASM_B4_MASK;
        case CASM_ERR_MASK:
            return CASM_ERR_MASK;
        default:
            return 0; /* TYPE B1 */
    }

}

casmInstructionFrame_t encodeInstruction(const char* inst,uint16_t type, int* instrLen)
{
    char buffer[3][20] = {0};
    char* ptr[3] = {buffer[0],buffer[1],buffer[2]};
    casmInstructionFrame_t frame = {0};

    switch(type){
        case 0:
            {
                tokenizeInstructionTypeB1(inst,ptr,instrLen);
                frame = getEncodedInstruction(ptr, 0);
            }
            break;
        case CASM_B2_MASK:
            {
                tokenizeInstructionTypeB2(inst,ptr,instrLen);
                frame = getEncodedInstruction(ptr, CASM_B2_MASK);
            }
            break;
        case CASM_B3_MASK:
            {
                tokenizeInstructionTypeB3(inst,ptr,instrLen);
                frame = getEncodedInstruction(ptr, CASM_B3_MASK);
            }
            break;
        case CASM_B4_MASK:
            {
                tokenizeInstructionTypeB4(inst,ptr,instrLen);
                frame = getEncodedInstruction(ptr, CASM_B4_MASK);
            }
            break;
        default:
            break;
    }
    return frame;
}
