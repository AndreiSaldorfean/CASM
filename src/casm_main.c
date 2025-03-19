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
    if (!fp) {
        perror("Error opening file");
        casmDestroyDict();
        return 1;
    }
    const char *instruction = "MOV 12(R1), R2";
    const char instArr[6][20] = {
        "MOV 12(R1), R2",
        "ADD R1, R2",
        "SUB R1, R2",
        "CMP R1, R2",
        "AND R1, R2",
        "OR R1, R2",
    };
    for( int i = 0; i < 6; i++) {
        uint16_t instructionType = getInstructionType(instArr[i]);
        casmInstructionFrame_t instr = encodeInstruction(instArr[i],instructionType);
        if(instr.instr) { fwrite(&instr.instr,2,1,fp); }
        if(instr.offset1) { fwrite(&instr.offset1,2,1,fp); }
        if(instr.offset2) { fwrite(&instr.offset2,2,1,fp); }
    }

    fclose(fp);
    casmDestroyDict();
    return 0;
}
