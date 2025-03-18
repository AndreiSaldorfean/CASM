#include "casm_utils.h"
#include "re.h"

void initGlobals(void){
    // Initialize the global variables
}

uint8_t identifyInstruction(const char* inst)
{
    re_t pattern = re_compile("(\\w\\d)$");
    int match = re_matchp(pattern, inst, 0);
    return 0;
}