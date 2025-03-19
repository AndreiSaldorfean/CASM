#include <stdio.h>
#include "re.h"
#include <string.h>
#include "casm_dict.h"
#include "casm_utils.h"
#include "casm_encoder.h"

int main(int *args, char **argv)
{
    casmInitDict();
    FILE* fp = fopen("main.obj","wb");
    FILE* fp2 = fopen("main.s","rb");
    system("dir");
    if (!fp) {
        perror("Error opening file");
        casmDestroyDict();
        return 1;
    }
    char instBuffer[30] = {0};

    while(fgets(instBuffer,20,fp2) != NULL) {
        uint16_t instructionType = getInstructionType(instBuffer);
        casmInstructionFrame_t instr = encodeInstruction(instBuffer,instructionType);
        print_binary(instr.instr);
        printf("\t%X\t%s",instr.instr,instBuffer);
    }

    fclose(fp);
    fclose(fp2);
    casmDestroyDict();
    return 0;
}
