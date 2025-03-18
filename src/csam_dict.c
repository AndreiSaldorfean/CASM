#include "casm_dict.h"
#include "casm_inst_formats.h"

khash_t(PROGRAM) *h = NULL;

static void casmInsertDict(uint16_t key,const char* value)
{
    int ret;
    khiter_t k = kh_put(PROGRAM, h, value, &ret);
    kh_val(h, k) = key;
}

void casmInitDict(void)
{
    h = kh_init(PROGRAM);
    for(int i = 0; i < CASM_DICT_SIZE; i++) {
        casmInsertDict(gInstructionsTable[i].opcode,gInstructionsTable[i].name);
    }
}

uint16_t casmGetDict(const char* value)
{
    khiter_t k = kh_get(PROGRAM, h, value);  // Lookup
    if (k != kh_end(h))
        return kh_val(h, k);
}

void casmDestroyDict(void)
{
    kh_destroy(PROGRAM, h);
}
