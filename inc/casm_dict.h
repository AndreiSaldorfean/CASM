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

KHASH_MAP_INIT_STR(PROGRAM, uint16_t)
extern khash_t(PROGRAM) *h;

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

void casmInitDict(void);
uint16_t casmGetDict(const char* value);
void casmDestroyDict(void);

#ifdef __cplusplus
}
#endif

#endif /* DICT_H */