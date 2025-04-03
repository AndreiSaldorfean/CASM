#include <stdint.h>
#include <string.h>
#include "re.h"
#include "casm_utils.h"
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

CASM_STATIC int find(const char* str ,char delimiter)
{
    int i = 0;
    for(; str[i]; i++)
    {
        if(str[i] == delimiter)
        {
            i++;
            return i - 1;
        }
    }
    if (str[i] == delimiter)
    {
        return i;
    }
    return -1;
}

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

int tokenizeInstructionTypeB1(const char* inst, char** destination) {
    int prevIndex = 0;
    int index = find(inst, ' ');
    if(index == -1) {
        return -1;
    }

    memcpy(destination[0], inst, index);

    prevIndex = index + 1;
    index = find(inst, ',');
    if(index == -1) {
        return -1;
    }
    memcpy(destination[1], inst + prevIndex, index - prevIndex);
    trim_whitespace(destination[1]);

    prevIndex = index;
    index = find(inst, 0);
    if(index == -1) {
        return -1;
    }
    memcpy(destination[2], inst + prevIndex + 1, index - prevIndex);
    trim_whitespace(destination[2]);
}

int tokenizeInstructionTypeB2(const char* inst, char** destination) {
    int prevIndex = 0;
    int index = find(inst, ' ');
    if(index == -1) {
        return -1;
    }
    memcpy(destination[0], inst, index);

    prevIndex = index;
    index = find(inst, 0);
    if(index == -1) {
        return -1;
    }
    memcpy(destination[1], inst + prevIndex, index - prevIndex);
    trim_whitespace(destination[1]);
}

int tokenizeInstructionTypeB3(const char* inst, char** destination) {
    int index = find(inst, ' ');
    if(index == -1) {
        return -1;
    }
    memcpy(destination[0], inst, index);

    index = find(inst, 0);
    if(index == -1) {
        return -1;
    }
    memcpy(destination[1], inst + index, index - 1);
    trim_whitespace(destination[1]);
}

int tokenizeInstructionTypeB4(const char* inst, char** destination) {
    int index = find(inst, 0);
    if(index == -1) {
        return -1;
    }
    memcpy(destination[0], inst, index);
    trim_whitespace(destination[0]);
}
