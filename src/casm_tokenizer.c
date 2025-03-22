#include <stdint.h>
#include <string.h>
#include "re.h"
#include "casm_utils.h"

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
extern int gIndex;
/* ============================================================================================================
                                            Local functions
============================================================================================================ */

static int tokenize(const char* str ,char delimiter)
{
    int i=gIndex;
    for(; str[gIndex]; gIndex++)
    {
        if(str[gIndex] == delimiter)
        {
            gIndex++;
            return gIndex - i - 1;
        }
    }
    return -1;
}

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

void tokenizeInstructionTypeB1(const char* inst, char** destination, int* instrLen) {
    int prevIndex = gIndex;
    int len = tokenize(inst, ' ');
    memcpy(destination[0], inst + prevIndex, len);
    trim_whitespace(destination[0]);
    destination[0][len+1] = '\0';

    prevIndex = gIndex;
    len = tokenize(inst, ',');
    memcpy(destination[1],inst + prevIndex, len);
    trim_whitespace(destination[1]);
    destination[1][len+1] = '\0';

    prevIndex = gIndex;
    len = tokenize(inst, '\n');
    memcpy(destination[2], inst + prevIndex, len);
    trim_whitespace(destination[2]);
    destination[2][len+1] = '\0';
    *instrLen = prevIndex + len + 1;
}

void tokenizeInstructionTypeB2(const char* inst, char** destination, int* instrLen) {
    int prevIndex = gIndex;
    int len = tokenize(inst, ' ');
    memcpy(destination[0], inst + prevIndex, len);
    trim_whitespace(destination[0]);
    destination[0][len+1] = '\0';

    prevIndex = gIndex;
    len = tokenize(inst, '\n');
    memcpy(destination[1], inst + prevIndex, len);
    trim_whitespace(destination[1]);
    *instrLen = prevIndex + len + 1;
}

void tokenizeInstructionTypeB3(const char* inst, char** destination, int* instrLen) {
    int prevIndex = gIndex;
    int len = tokenize(inst, ' ');
    memcpy(destination[0], inst + prevIndex, len);
    destination[0][len+1] = '\0';
    trim_whitespace(destination[0]);

    prevIndex = gIndex;
    len = tokenize(inst, '\n');
    memcpy(destination[1], inst + prevIndex, len);
    trim_whitespace(destination[1]);
    *instrLen = prevIndex + len + 1;
}

void tokenizeInstructionTypeB4(const char* inst, char** destination, int* instrLen) {
    int prevIndex = gIndex;
    int len = tokenize(inst, '\n');
    memcpy(destination[0], inst + prevIndex, len);
    destination[0][len+1] = '\0';
    trim_whitespace(destination[0]);
    *instrLen = prevIndex + len + 1;
}
