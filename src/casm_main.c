#include <stdio.h>
#include "re.h"
#include <string.h>
#include "casm_dict.h"
#include "casm_utils.h"
#include "casm_encoder.h"
#include "getopt.h"
#include "casm_preprocessor.h"

uint8_t debugEnabled = 0;

int main(int argc, char **argv)
{
    FILE* fpInput = NULL;
    FILE* fpOutput = NULL;
    int opt;
    char output_file[30] = {0};
    char instBuffer[30] = {0};
    char buffer0[500] = {0};
    char processedFile[500] = {0};
    uint16_t buffer1[200] = {0};
    uint32_t size = 0;
    int instrLen = 0;
    casmInitDict();

    if(argc == 1)
    {
        printf(CASM_HELP_MESSAGE);
        casmDestroyDict();
        return 1;
    }

    if((argc == 2 || argc >= 3) && (argv[1][0] != '-'))
    {
       fpInput = fopen(argv[1],"rb");
       fread(buffer0,1,500,fpInput);
       preprocessFile(buffer0, processedFile);
       optind = 2;
    }

    if(argc == 2 && (argv[1][0] != '-'))
    {
        int len = 0;
        re_match("^\\S+\\.",argv[1],&len);
        memcpy(output_file,argv[1],len-1);
        memcpy(output_file + len - 1,".obj",4);
        output_file[len + 3] = '\0';

        fpOutput = fopen(output_file,"wb");
        if (fpOutput == NULL) {
            fprintf(stderr,"Error opening file\n");
            return 1;
        }
    }

    while ((opt = getopt(argc, argv,"Pvho:")) != -1)
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
                fpOutput = fopen(argv[3],"wb");
                break;
            case 'v':
                debugEnabled = 1;
                break;
            case 'P':
                printf("Preprocessed File:\n");
                printf("%s",processedFile);
                break;
            case '?':
                casmDestroyDict();
                return 1;
        }
    }


    while(1)
    {
        uint16_t instructionType = getInstructionType(processedFile+instrLen);
        if(instructionType == CASM_STOP)
        {
            printf("\nTotal size: %d bytes\n",size*2);
            fwrite(buffer1,1,size*2+1,fpOutput);
            break;
        }
        if(instructionType == CASM_ERR_MASK)
        {
            fprintf(stderr,"\nError: Invalid instruction encountered: %s\n\nOr no HALT instruction was added at the end of the program!\n", instBuffer);
            break;
        }
        int prevLen = instrLen;
        casmInstructionFrame_t instr = encodeInstruction(processedFile,instructionType, &instrLen);

        if(instr.instr != 0)
        {
            memcpy(buffer1+size,&instr.instr,2);

            if(debugEnabled)
            {
                printf("0x%X\t",size*2);
                print_binary(instr.instr);
                char buffer2[30] = {0};
                memcpy(buffer2,processedFile+prevLen,instrLen-prevLen);
                printf("\t0x%X\t%s",instr.instr,buffer2);
            }
            size++;
        }
        if(instr.offset1 != 0)
        {
            memcpy(buffer1+size,&instr.offset1,2);

            if (debugEnabled)
            {
                printf("0x%X\t",size*2);
                print_binary(instr.offset1);
                printf("\t0x%X\t%d\n",instr.offset1, instr.offset1);
            }
            size++;
        }
        if(instr.offset2 != 0)
        {
            memcpy(buffer1+size,&instr.offset2,2);

            if (debugEnabled)
            {
                printf("0x%X\t",size*2);
                print_binary(instr.offset2);
                printf("\t0x%X\t%d\n",instr.offset2, instr.offset2);
            }
            size++;
        }

    }
    printf("\n");

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