#include <stdio.h>
#include "re.h"
#include <string.h>
#include "casm_dict.h"
#include "casm_utils.h"
#include "casm_encoder.h"
#include "getopt.h"
#include "casm_preprocessor.h"

#define ADDR_PRINT          "%-3X"
#define BINARY_INST_PRINT   "|%-22s|"
#define HEX_INST_PRINT      "%-7X|"
#define TEXT_INST_PRINT     "%-22s|"
#define OFFSET_PRINT        "%-22d|"
#define LABEL_PRINT         "%s"

uint8_t debugEnabled = 0;

int main(int argc, char **argv)
{
    FILE* fpInput = NULL;
    FILE* fpOutput = NULL;
    casm_program_t program[100] = {0};
    int opt;
    char output_file[30] = {0};
    char instBuffer[30] = {0};
    char buffer0[500] = {0};
    uint16_t buffer1[200] = {0};
    uint32_t size = 0;
    int instrLen = 0;
    char binary[20] = {0};
    int programSize = 0;
    casmInitInstructionTable();

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
       preprocessFile(buffer0, program, &programSize);
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

    while ((opt = getopt(argc, argv,"Svho:")) != -1)
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
            case 'S':
                printf("\033[1;37mSymbol Table\e[0m\n");
                for(int i = 0; gSymbolTable[i].label[0]; i++)
                {
                    printf("0x"ADDR_PRINT"| %s\n",gSymbolTable[i].address,gSymbolTable[i].label);
                }
                break;
            case '?':
                casmDestroyDict();
                return 1;
        }
    }

    printf("\033[1;37m%-5s|%-22s|%s|%-23s|%s\e[0m\n","Addr"," Binary Inst", " Hex Inst "," Text Inst"," Labels");
    for(int i = 0; i < programSize; i++)
    {
        char* instruction = program[i].instruction;

        uint16_t instructionType = getInstructionType(instruction);
        if(instructionType == CASM_ERR_MASK)
        {
            fprintf(stderr,"\n\033[0;31mError: Invalid instruction encountered: %s\e[0m\n\nOr no HALT instruction was added at the end of the program!\n", instBuffer);
            break;
        }

        casmInstructionFrame_t instr = encodeInstruction(instruction,instructionType);

        if(instr.instr != 0)
        {
            memcpy(buffer1+size,&instr.instr,2);

            if(debugEnabled)
            {
                int ok = 0;
                for(int j=0;gSymbolTable[j].label[0];j++)
                {
                    if(gSymbolTable[j].address == program[i].address)
                    {
                        ok = 1;
                        get_binary_num(instr.instr, binary);

                        printf(
                            "\e[0;35m0x"ADDR_PRINT"\e[0m" \
                            BINARY_INST_PRINT
                            " 0x"HEX_INST_PRINT" "TEXT_INST_PRINT
                            "\e[0;35m "LABEL_PRINT":\e[0m\n",
                            program[i].address,
                            binary,
                            instr.instr,
                            instruction,
                            gSymbolTable[j].label
                        );
                        break;
                    }
                }

                if(ok == 0)
                {
                    get_binary_num(instr.instr, binary);
                    if(program[i].label[0] != 0)
                    {
                        printf(
                            "\033[0;90m0x"ADDR_PRINT"\e[0m"BINARY_INST_PRINT
                            " 0x"HEX_INST_PRINT" "TEXT_INST_PRINT
                            "\033[0;90m "LABEL_PRINT"\e[0m\n",
                            program[i].address,
                            binary,
                            instr.instr,
                            instruction,
                            program[i].label
                        );
                    }
                    else
                    {
                        printf(
                            "0x"ADDR_PRINT BINARY_INST_PRINT
                            " 0x"HEX_INST_PRINT" "TEXT_INST_PRINT"\n",
                            program[i].address,
                            binary,
                            instr.instr,
                            instruction
                        );
                    }
                }
            }
            size++;
        }
        if(instr.offset1 != 0)
        {
            memcpy(buffer1+size,&instr.offset1,2);

            if (debugEnabled)
            {
                get_binary_num(instr.offset1, binary);

                printf(
                    "0x"ADDR_PRINT BINARY_INST_PRINT
                    " 0x"HEX_INST_PRINT" "OFFSET_PRINT"\n",
                    (program[i].address + 2),
                    binary,
                    instr.offset1,
                    instr.offset1
);
            }
            size++;
        }
        if(instr.offset2 != 0)
        {
            memcpy(buffer1+size,&instr.offset2,2);

            if (debugEnabled)
            {
                get_binary_num(instr.offset2, binary);

                printf(
                    "0x"ADDR_PRINT BINARY_INST_PRINT
                    " 0x"HEX_INST_PRINT" "OFFSET_PRINT"\n",
                    (program[i].address + 4),
                    binary,
                    instr.offset2,
                    instr.offset2
                );
            }
            size++;
        }

    }

    printf("\nTotal size: %d bytes\n",size*2);
    fwrite(buffer1,1,size*2+1,fpOutput);

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