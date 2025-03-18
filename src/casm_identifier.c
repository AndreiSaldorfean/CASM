#include "casm_dict.h"
#include "re.h"

uint8_t identifyInstruction(const char* inst)
{
    re_t pattern = re_compile("(\\w\\d)$");
    int match = re_matchp(pattern, inst, 0);
    return 0;
}