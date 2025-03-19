#include <stdio.h>
#include "re.h"
#include <string.h>
#include "casm_dict.h"
#include "casm_utils.h"
#include "casm_encoder.h"
// #include "getopt.h" // not available for clang

int main(int argc, char **argv)
{
    FILE* fpInput = NULL;
    FILE* fpOutput = NULL;
    int opt;
    char input_file[30];
    char output_file[30];
    char instBuffer[30] = {0};
    casmInitDict();

    if((argc == 2 || argc >= 3) && (argv[1][0] != '-'))
    {
       fopen_s(&fpInput,argv[1],"r+");
       argIndex = 2;
    }

    if(argc == 2 && (argv[1][0] != '-'))
    {
        int len = 0;
        re_match("^\\S+\\.",argv[1],&len);
        memcpy(output_file,argv[1],len-1);
        memcpy(output_file + len - 1,".obj",4);
        output_file[len + 3] = '\0';

        errno_t err = fopen_s(&fpOutput,output_file,"w+");
        if (err != 0 || fpOutput == NULL) {
            perror("Error opening file");
            return 1;
        }
    }

    if (fpInput == NULL && (argv[1][0] != '-'))
    {
        fprintf(stderr,"Error opening file\n");
        casmDestroyDict();
        return 1;
    }

    while ((opt = parse_args(argc, argv)) != 0xFFU)
    {
        switch (opt)
        {
            case 'h':
                printf(CASM_HELP_MESSAGE);
                casmDestroyDict();
                return 0;
            case 'o':
                if (argc < 4)
                {
                    fprintf(stderr,"Usage: casm input_file -o output_file\n");
                    return 1;
                }
                fopen_s(&fpOutput ,argv[argIndex],"w");
                break;
            case '?':
                casmDestroyDict();
                return 1;
        }
    }

    while(fgets(instBuffer,20,fpInput) != NULL)
    {
        uint16_t instructionType = getInstructionType(instBuffer);
        if(instructionType == CASM_ERR_MASK)
        {
            fprintf(stderr,"Error: Invalid instruction\n");
            casmDestroyDict();
            return 1;
        }
        casmInstructionFrame_t instr = encodeInstruction(instBuffer,instructionType);

        if(instr.instr != 0)
        {
            fwrite(&instr.instr,2,1,fpOutput);

            print_binary(instr.instr);
            printf("\t%X\t%s",instr.instr,instBuffer);
        }
        if(instr.offset1 != 0)
        {
            fwrite(&instr.offset1,2,1,fpOutput);

            printf("%d %X\n",instr.offset1, instr.offset1);
        }
        if(instr.offset2 != 0)
        {
            fwrite(&instr.offset2,2,1,fpOutput);

            printf("%d %X\n",instr.offset2, instr.offset2);
        }

    }

    if(fpOutput)
    {
        fclose(fpOutput);
    }

    if(fpInput)
    {
        fclose(fpInput);
    }

    casmDestroyDict();
    return 0;
}
