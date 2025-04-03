#include "casm_dict.h"
#include <ctype.h>
#include "casm_inst_formats.h"
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
khash_t(INST_TABLE) *hIns  = NULL;
khash_t(SYM_TABLE)  *hSym  = NULL;
khash_t(REG_TABLE)  *hReg  = NULL;
casm_addressingtLookUpTable_t gAddressingTable[] = { ADDRESSING_TABLE_VALUES };
casm_instLookUpTable_t gInstructionsTable[]      = { INTRUCTIONS_TABLE_VALUES };
casm_regisertLookUpTable_t gRegisterTable[]      = { REGISTER_TABLE_VALUES };
/* ============================================================================================================
                                            Local functions
============================================================================================================ */

CASM_STATIC void casmSetInstr(uint16_t key,const char* value)
{
    int ret;
    khiter_t k = kh_put(INST_TABLE, hIns, value, &ret);
    kh_val(hIns, k) = key;
}

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

void casmSetReg(int key,const char* value)
{
    int ret;
    khiter_t k = kh_put(REG_TABLE, hReg, value, &ret);
    kh_val(hReg, k) = key;
}

int casmGetReg(const char* value)
{
    char buffer[10] = {0};
    for(int i = 0; value[i]; i++) {
        buffer[i] = toupper(value[i]);
    }

    khiter_t k = kh_get(REG_TABLE, hReg, buffer);
    if (k != kh_end(hReg))
        return kh_val(hReg, k);
    return -1;
}

void casmSetSym(int key,const char* value)
{
    int ret;
    khiter_t k = kh_put(SYM_TABLE, hSym, value, &ret);
    kh_val(hSym, k) = key;
}

int casmGetSym(const char* value)
{
    khiter_t k = kh_get(SYM_TABLE, hSym, value);
    if (k != kh_end(hSym))
        return kh_val(hSym, k);
    return -1;
}

void casmInitRegTable(void)
{
    hReg = kh_init(REG_TABLE);
    for(uint16_t i = 0; i < 15; i++) {
        casmSetReg(gRegisterTable[i].index ,gRegisterTable[i].name);
    }
}

void casmInitInstructionTable(void)
{
    hIns = kh_init(INST_TABLE);
    for(uint16_t i = 0; i < CASM_DICT_SIZE; i++) {
        casmSetInstr(gInstructionsTable[i].opcode,gInstructionsTable[i].name);
    }
}

uint16_t csamGetInstruction(const char* value)
{
    char buffer[7] = {0};
    for(int i = 0; value[i]; i++) {
        buffer[i] = toupper(value[i]);
    }

    khiter_t k = kh_get(INST_TABLE, hIns, buffer);  // Lookup
    if (k != kh_end(hIns))
        return kh_val(hIns, k);
    return -1;
}

void casmDestroyDict(void)
{
    kh_destroy(INST_TABLE, hIns);
    kh_destroy(SYM_TABLE, hSym);
    kh_destroy(REG_TABLE, hReg);
}
