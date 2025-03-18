#include <stdint.h>
#include <string.h>
#include "re.h"

char* tokenizeInstructionTypeB1(const char* inst) {
    re_t pattern = re_compile("(\\w\\d)$");
    const char* text = "MOV R0, (R1)";
    int match_len;
    int match = re_matchp(pattern, inst, &match_len);

    if(match != -1) {
        char buffer[7] = {0};
        strncpy(buffer, text + match, match_len);
        buffer[match_len] = '\0';
        printf("Match: %s\n", buffer);
    } else {
        printf("No match\n");
    }
}