#include "casm_encoder.h"
#include <stdint.h>
#include "casm_tokenizer.h"
#include "casm_utils.h"
#include "re.h"
#include <stdio.h>
#include "casm_dict.h"
#include "casm_config.h"


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
CASM_STATIC int isRegiserValid(const char* reg)
{
    char regBuf[10] = {0};
    int match_len = 0;
    int match = re_match(CASM_REGISTER_RGX, reg, &match_len);
    if(match == -1){
        return -1;
    }
    memcpy(regBuf, reg + match, match_len);
    return casmGetReg(regBuf);
}

CASM_STATIC uint16_t getRegisterIdx(const char* reg)
{
    char buf[10] = {0};
    int len = 0;
    int offset = re_match(CASM_REGISTER_RGX,reg,&len);
    if( offset == -1){
        return 0;
    }
    memcpy(buf, reg + offset + 1, len - 1);
    return ascii_to_int(buf);
}

CASM_STATIC int getAddressingMode(const char* reg)
{
    int match_len = 0;
    int match = re_match(CASM_IMM_AM_RGX, reg, &match_len);
    if(match != -1){
        return 0;
    }

    match = re_match(CASM_NEG_IMM_AM_RGX, reg, &match_len);
    if(match != -1){
        return 0;
    }

    int res = isRegiserValid(reg);
    if(res == -1){
        return 0xFF;
    }

    match = re_match(CASM_DIRECT_AM_RGX, reg, &match_len);
    if(match != -1){
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

    return -1;
}

CASM_STATIC casmInstructionFrame_t getEncodedInstruction(char** instructionTokens, uint16_t instrType)
{
    casmInstructionFrame_t frame = {0};
    int off = 0;
    int offset = 0;
    uint8_t reg = 0;
    uint16_t idx = 0;

    switch (instrType)
    {
        case 0:
        {
            /*--------------------------- SOURCE ------------------------------*/
            uint8_t ams = getAddressingMode(instructionTokens[2]);

            switch(ams)
            {
                case 0:
                {
                    uint16_t num = ascii_to_int(instructionTokens[2]);
                    frame.offsetSource = num;
                    break;
                }
                case 3:
                {
                    off = 0;
                    offset = re_match("\\d+\\[",instructionTokens[2],&off);
                    char buffer[4] = {0};
                    memcpy(buffer, instructionTokens[2] + offset, off-1);

                    long num = strtol(buffer, NULL, 10);
                    frame.offsetSource = num;
                    break;
                }
                case 0xFF/* ERROR - Invalid register */:
                {
                    return (casmInstructionFrame_t){.instr = 0xFFFF};
                    break;
                }
            }

            //Add the addressing mode to the instruction
            frame.instr |= (ams<<10);

            //Add the source register to the instruction
            idx = getRegisterIdx(instructionTokens[2]);
            frame.instr |= (idx<<6);

            /*--------------------------- DESTINATION ------------------------------*/
            uint8_t amd = getAddressingMode(instructionTokens[1]);
            switch (amd)
            {
                case 0:
                {
                    /* Cannot store in a constant*/
                    return (casmInstructionFrame_t){.instr = 0xFFFF};
                }
                case 3:
                {
                    off = 0;
                    offset = re_match("\\d+\\[",instructionTokens[1],&off);
                    char buffer[4] = {0};
                    memcpy(buffer, instructionTokens[1] + offset, off-1);

                    long num = strtol(buffer, NULL, 10);
                    frame.offsetDestination = num;
                    break;
                }
                case 0xFF/* ERROR - Invalid register */:
                {
                    return (casmInstructionFrame_t){.instr = 0xFFFF};
                }
            }

            //Add the addressing mode to the instruction
            frame.instr |= (amd<<4);

            idx = getRegisterIdx(instructionTokens[1]);
            frame.instr |= idx;
            break;

        }

        case CASM_B2_MASK:
        {
            uint8_t amd = getAddressingMode(instructionTokens[1]);
            switch (amd)
            {
                case 0:
                {
                    uint16_t num = ascii_to_int(instructionTokens[1]);
                    frame.offsetSource = num;
                    break;
                }
                case 3:
                {
                    off = 0;
                    offset = re_match("\\d+\\[",instructionTokens[1],&off);
                    char buffer[4] = {0};
                    memcpy(buffer, instructionTokens[1] + offset, off-1);

                    long num = strtol(buffer, NULL, 10);
                    frame.offsetSource = num;
                }
                case 0xFF/* ERROR - Invalid register */:
                {
                    return (casmInstructionFrame_t){.instr = 0xFFFF};
                }
            }

            frame.instr |= (amd<<4);
            frame.instr |= getRegisterIdx(instructionTokens[1]);
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

    uint16_t opcode = csamGetInstruction(instructionTokens[0]);
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

    memcpy(buffer, inst + match, size);
    buffer[size] = '\0';

    trim_whitespace(buffer);

    switch (csamGetInstruction(buffer) & CASM_ERR_MASK)
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

casmInstructionFrame_t encodeInstruction(const char* inst,uint16_t type)
{
    char buffer[3][20] = {0};
    char* ptr[3] = {buffer[0],buffer[1],buffer[2]};
    int result = 0;
    casmInstructionFrame_t frame = {0};

    switch(type){
        case 0:
            {
                result = tokenizeInstructionTypeB1(inst,ptr);
                frame = getEncodedInstruction(ptr, 0);
            }
            break;
        case CASM_B2_MASK:
            {
                result = tokenizeInstructionTypeB2(inst,ptr);
                frame = getEncodedInstruction(ptr, CASM_B2_MASK);
            }
            break;
        case CASM_B3_MASK:
            {
                result = tokenizeInstructionTypeB3(inst,ptr);
                frame = getEncodedInstruction(ptr, CASM_B3_MASK);
            }
            break;
        case CASM_B4_MASK:
            {
                result = tokenizeInstructionTypeB4(inst,ptr);
                frame = getEncodedInstruction(ptr, CASM_B4_MASK);
            }
            break;
        default:
            break;
    }
    if (result == -1)
    {
        frame.instr = 0xFFFF;
    }

    return frame;
}
