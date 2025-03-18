#include <stdio.h>
#include "re.h"
#include <string.h>
#include "casm_dict.h"
#include "casm_globals.h"


int main(int *args, char **argv)
{
    casmInitDict();
    initGlobals();
    printf("%d",casmGetDict("CLC"));
    re_t pattern = re_compile("(\\w\\d)$");
    const char* text = "MOV R0, (R1)";
    int match_len;
    int match = re_matchp(pattern, text, &match_len);

    if(match != -1) {
        char buffer[7] = {0};
        strncpy(buffer, text + match, match_len);
        buffer[match_len] = '\0';
        printf("Match: %s\n", buffer);
    } else {
        printf("No match\n");
    }
    const char* inst1 = "MOV R0, R1";
    const char* inst2 = "MOV R4,(R2)";
    casmDestroyDict();
    return 0;
}