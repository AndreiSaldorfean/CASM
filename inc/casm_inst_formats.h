#ifndef OPPCODES_H
#define OPPCODES_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

/**
 * @brief  Enumerations for the CASM instructions
 * @note   The CASM instructions are divided into 4 types
 *       B1, B2, B3, B4
 * @details
 *      15 14 13                         0
 *      0  x  x  x x x x x x x x x x x x x
 *      ^ This marks that the instruction is of type B1
 *
 *      15 14 13                         0
 *      1  x  1  x x x x x x x x x x x x x
 *      ^ This marks that the instruction is of type B2
 *
 *      15 14 13                         0
 *      1  1  x  x x x x x x x x x x x x x
 *      ^ ^ This marks that the instruction is of type B3
 *
 *      15 14 13                         0
 *      1  1  1  x x x x x x x x x x x x x
 *      ^ ^ ^ This marks that the instruction is of type B4
 */
typedef enum
{
    /** B1 INSTRUCTIONS TYPE
     *  MOV, ADD, SUB, CMP, AND, OR, XOR
     *  OPCODE: Operation Code - 4b
     *  AMS: Addressing Mode Source - 2b
     *  SR: Source Register - 4b
     *  AMD: Addressing Mode Destination - 2b
     *  DR: Destination Register - 4b
     *  OPPCODE | AMS | SR | AMD | DR
    */
    CASM_MOV = 0x0000, /* 0x0 */
    CASM_ADD = 0x1000,
    CASM_SUB = 0x2000,
    CASM_CMP = 0x3000,
    CASM_AND = 0x4000,
    CASM_OR  = 0x5000,
    CASM_XOR = 0x6000,
    /**  B2 INSTRUCTIONS TYPE
     * CLR, NEG, INC, DEC, ASL, ASR, LSR, ROL, ROR, RLC, RRC
     * OPCODE: Operation Code - 10b
     * AMD: Addressing Mode Destination - 2b
     * SD: Destination Register - 4b
     * OPPCODE | AMD | DR
    */
    CASM_CLR  = 0xA000, /* 0x800 */
    CASM_NEG  = 0xA040,
    CASM_INC  = 0xA0C0,
    CASM_DEC  = 0xA100,
    CASM_ASL  = 0xA140,
    CASM_ASR  = 0xA1C0,
    CASM_LSR  = 0xA200,
    CASM_ROL  = 0xA240,
    CASM_ROR  = 0xA2C0,
    CASM_RLC  = 0xA300,
    CASM_RRC  = 0xA340,
    CASM_JMP  = 0xA3C0,
    CASM_CALL = 0xA400,
    CASM_PUSH = 0xA440,
    CASM_POP  = 0xA4C0,
    /** B3 INSTRUCTIONS TYPE
     * BR, BNE, BEQ, BPL, BMI, BCS, BCC, BVS, BVC
     * OPCODE: Operation Code - 8b
     * OFFSET: Offset - 8b
     * OPPCODE | OFFSET
     */
    CASM_BR  = 0xC000, /* 0xC0 */
    CASM_BNE = 0xC100,
    CASM_BEQ = 0xC200,
    CASM_BPL = 0xC300,
    CASM_BMI = 0xC400,
    CASM_BCS = 0xC500,
    CASM_BCC = 0xC600,
    CASM_BVS = 0xC700,
    CASM_BVC = 0xC800,
    /** B4 INSTRUCTIONS TYPE
     * CLC, CLV, CLZ, CLS, CCC, SEC, SEV, SEZ, SES, SCC, NOP, RET, RETI, HALT,
     * WAIT, PUSHPC, POPPC, PUSHF, POPF
     * OPCODE: Operation Code - 16b
     * OPPCODE
     */
    CASM_CLC = 0xE000, /* 0xE00 */
    CASM_CLV,
    CASM_CLZ,
    CASM_CLS,
    CASM_CCC,
    CASM_SEC,
    CASM_SEV,
    CASM_SEZ,
    CASM_SES,
    CASM_SCC,
    CASM_NOP,
    CSAM_RET,
    CASM_RETI,
    CASM_HALT,
    CASM_WAIT,
    CASM_PUSHPC,
    CASM_POPPC,
    CASM_PUSHF,
    CASM_POPF,
}casm_opcodes_t;

typedef enum
{
    CASM_IMM = 0,
    CASM_DA,
    CASM_IA,
    CASM_XA,
}casm_addrModes_t;

typedef enum
{
    CASM_REG0 = 0,
    CASM_REG1,
    CASM_REG2,
    CASM_REG3,
    CASM_REG4,
    CASM_REG5,
    CASM_REG6,
    CASM_REG7,
    CASM_REG8,
    CASM_REG9,
    CASM_REG10,
    CASM_REG11,
    CASM_REG12,
    CASM_REG14,
    CASM_REG15,
}casm_registers_t;

#define INTRUCTIONS_TABLE_VALUES   \
    {   "MOV",      CASM_MOV    }, \
    {   "ADD",      CASM_ADD    }, \
    {   "SUB",      CASM_SUB    }, \
    {   "CMP",      CASM_CMP    }, \
    {   "AND",      CASM_AND    }, \
    {   "OR",       CASM_OR     }, \
    {   "XOR",      CASM_XOR    }, \
    {   "CLR",      CASM_CLR    }, \
    {   "NEG",      CASM_NEG    }, \
    {   "INC",      CASM_INC    }, \
    {   "DEC",      CASM_DEC    }, \
    {   "ASL",      CASM_ASL    }, \
    {   "ASR",      CASM_ASR    }, \
    {   "LSR",      CASM_LSR    }, \
    {   "ROL",      CASM_ROL    }, \
    {   "ROR",      CASM_ROR    }, \
    {   "RLC",      CASM_RLC    }, \
    {   "RRC",      CASM_RRC    }, \
    {   "JMP",      CASM_JMP    }, \
    {   "CALL",     CASM_CALL   }, \
    {   "PUSH",     CASM_PUSH   }, \
    {   "POP",      CASM_POP    }, \
    {   "BR",       CASM_BR     }, \
    {   "BNE",      CASM_BNE    }, \
    {   "BEQ",      CASM_BEQ    }, \
    {   "BPL",      CASM_BPL    }, \
    {   "BMI",      CASM_BMI    }, \
    {   "BCS",      CASM_BCS    }, \
    {   "BCC",      CASM_BCC    }, \
    {   "BVS",      CASM_BVS    }, \
    {   "BVC",      CASM_BVC    }, \
    {   "CLC",      CASM_CLC    }, \
    {   "CLV",      CASM_CLV    }, \
    {   "CLZ",      CASM_CLZ    }, \
    {   "CLS",      CASM_CLS    }, \
    {   "CCC",      CASM_CCC    }, \
    {   "SEC",      CASM_SEC    }, \
    {   "SEV",      CASM_SEV    }, \
    {   "SEZ",      CASM_SEZ    }, \
    {   "SES",      CASM_SES    }, \
    {   "SCC",      CASM_SCC    }, \
    {   "NOP",      CASM_NOP    }, \
    {   "RET",      CSAM_RET    }, \
    {   "RETI",     CASM_RETI   }, \
    {   "HALT",     CASM_HALT   }, \
    {   "WAIT",     CASM_WAIT   }, \
    {   "PUSHPC",   CASM_PUSHPC }, \
    {   "POPPC",    CASM_POPPC  }, \
    {   "PUSHF",    CASM_PUSHF  }, \
    {   "POPF",     CASM_POPF   }, \

#define ADDRESSING_TABLE_VALUES \
    /* Handled differently */   \
    { "", CASM_IMM },           \
    { "()", CASM_DA },          \
    { "IA", CASM_IA },          \
    { "XA", CASM_XA },          \

#define REGISTER_TABLE_VALUES  \
    { "R0",     CASM_REG0   }, \
    { "R1",     CASM_REG1   }, \
    { "R2",     CASM_REG2   }, \
    { "R3",     CASM_REG3   }, \
    { "R4",     CASM_REG4   }, \
    { "R5",     CASM_REG5   }, \
    { "R6",     CASM_REG6   }, \
    { "R7",     CASM_REG7   }, \
    { "R8",     CASM_REG8   }, \
    { "R9",     CASM_REG9   }, \
    { "R10",    CASM_REG10  }, \
    { "R11",    CASM_REG11  }, \
    { "R12",    CASM_REG12  }, \
    { "R14",    CASM_REG14  }, \
    { "R15",    CASM_REG15  }, \

/* ============================================================================================================
                                            TYPEDEFS AND STRUCTS
============================================================================================================ */

typedef struct{
    char name[7];
    uint16_t opcode;
}casm_instLookUpTable_t;

typedef struct{
    char name[6];
    uint16_t moode;
}casm_addressingtLookUpTable_t;

typedef struct{
    char name[6];
    uint16_t index;
}casm_regisertLookUpTable_t;

/* ============================================================================================================
                                            Local Variables
============================================================================================================ */

/* ============================================================================================================
                                            Global Variables
============================================================================================================ */

extern casm_instLookUpTable_t gInstructionsTable[];
extern casm_addressingtLookUpTable_t gAddressingTable[];
extern casm_regisertLookUpTable_t gRegisterTable[];

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

/* ============================================================================================================
                                        TYPEDEFS AND STRUCTS
============================================================================================================ */

#ifdef __cplusplus
}
#endif

#endif /* OPPCODES_H */
