#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "re.h"
/* ============================================================================================================
                                            DEFINES
============================================================================================================ */

#define CASM_B1_MASK 0x8000
#define CASM_B2_MASK 0xC000
#define CASM_B3_MASK 0xE000
#define CASM_B4_MASK 0xF000

void initGlobals(void);
uint8_t identifyInstruction(const char* inst);

#ifdef __cplusplus
}
#endif
#endif /* UTILS_H */