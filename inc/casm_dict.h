#ifndef DICT_H
#define DICT_H

#ifdef __cplusplus
extern "C"{
#endif

#include "khash.h"
#include <stdint.h>

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

#define CASM_DICT_SIZE (50U)

/* ============================================================================================================
                                            TYPEDEFS AND STRUCTS
============================================================================================================ */

/* ============================================================================================================
                                            Local Variables
============================================================================================================ */

/* ============================================================================================================
                                            Global Variables
============================================================================================================ */

KHASH_MAP_INIT_STR(INST_TABLE, uint16_t)
KHASH_MAP_INIT_STR(SYM_TABLE, uint16_t)
extern khash_t(INST_TABLE) *hIns;
extern khash_t(SYM_TABLE) *hSym;

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

int casmGetSym(const char* value);
void casmSetSym(int key,const char* value);
void casmInitInstructionTable(void);
uint16_t csamGetInstruction(const char* value);
void casmDestroyDict(void);

#ifdef __cplusplus
}
#endif

#endif /* DICT_H */