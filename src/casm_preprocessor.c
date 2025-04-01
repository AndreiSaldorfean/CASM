#include "casm_preprocessor.h"
#include <stdio.h>
#include "re.h"
#include <string.h>
#include <ctype.h>
#include "casm_config.h"
#include "casm_dict.h"
#include "casm_utils.h"
#include "casm_encoder.h"
#include <ctype.h>

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
casm_symbolTable_t gSymbolTable[20] = { 0 };
/* ============================================================================================================
                                            Local functions
============================================================================================================ */

CASM_STATIC void trimWhiteSpaces(char* file, int* fileSize)
{
    char buffer[500] = {0};
    int j = 0;
    int i = 0;
    while(file[i] == '\n')i++;

    // Trim the newlines
    for(; i < *fileSize; i++)
    {
        if(
            (file[i] == '\n' && i == 0) ||
            (file[i] == '\n' && file[i+1] == '\n') ||
            (file[i] == ' '  && file[i+1] == ' ')  ||
            (file[i] == ' ' && file[i+1] == '\n')  ||
            (file[i] == ' ' && file[i-1] == ' ')
        )
        {
            continue;
        }
        buffer[j++] = file[i];
    }

    *fileSize = j;
    memcpy(file, buffer, j+1);
}

CASM_STATIC void getAsciiNumber(int number, char* result, int* size)
{
    int digit;
    char buf[6] = {0};
    int i = 0;
    int isSigned = 0;

    if (number > 128 || number <= -128) {
        fprintf(stderr, "Number is too large for signed byte: %d\n", number);
        *size = 0;
        return;
    }

    if (number == 0) {
        result[0] = '0';
        *size = 1;
        return;
    }

    if (number < 0) {
        result[0] = '-';
        number *= -1;
        i++;
        isSigned = 1;
    }
    while (number > 0) {
        digit = number % 10;
        result[i++] = digit + '0';
        number /= 10;
    }

    result = strrev(result + isSigned);
    *size = i;
}

CASM_STATIC void removeComments(char* file, int* fileSize)
{
    int i=0;
    int j=0;
    char destination[500] = {0};

    for(i=0; i < *fileSize; i++)
    {
        if (file[i] == ';')
        {
            while(file[i] != '\n')
            {
                i++;
            }

            if (file[i-1] == '\n' || file[i+1] == '\n' || j == 0 || destination[j-1] == '\n')
            {
                continue;
            }
        }

        destination[j++] = file[i];
    }
    *fileSize = j;
    memset(file, 0, 500);
    memcpy(file, destination, j+1);
}
CASM_STATIC void procsToLabels(char *file, int* fileSize)
{
    int len = 0;
    int match = 0;
    int match1 = 0;
    char labelBuffer[50] = {0};

    // Convert <proc> - <endp> to labels
    while(1)
    {
        match = re_match("proc ",file,&len);
        if(match == -1)
        {
            break;
        }
        memcpy(file + match, file + match + len - 1, *fileSize - match);
        *fileSize -= len - 1;
        for(int i = match+1; ; i++)
        {
            if(file[i] == '\n')
            {
                file[i-1] = ':';
                break;
            }
            file[i-1] = file[i];
        }
        match = re_match("endp",file,&len);
        for(int i = match; ; i++)
        {
            if(file[i] == '\n')
            {
                memcpy(file + match, file + i + 1, *fileSize - i);
                *fileSize -= i - match + 1;
                break;
            }
        }
    }
}

CASM_STATIC void labelsToAdresses(char* file, int* fileSize, casm_program_t* program, int* programSize)
{
    int len          = 0;
    int symbolIndex  = 0;
    int programIndex = 0;
    int labelAddres  = 0;
    int oldLine      = 0;
    int prevAddress  = 0;
    int match        = 0;

    hSym = kh_init(SYM_TABLE);

    for (int i = 0; i < *fileSize; i++)
    {
        if(file[i] == '\n')
        {
            labelAddres +=2;
            if (labelAddres == 0)
            {
                continue;
            }
            program[programIndex].address = prevAddress;
            memcpy(program[programIndex].instruction, file + oldLine, i - oldLine);
            prevAddress = labelAddres;
            oldLine = i + 1;
            programIndex++;
        }
        if ( file[i] == '[' && isdigit(file[i-1]))
        {
            labelAddres += 2;
        }
        if(file[i] == ':')
        {
            memcpy(gSymbolTable[symbolIndex].label, file + oldLine,  i - oldLine);
            gSymbolTable[symbolIndex++].address = labelAddres;
            *fileSize -= i - oldLine + 1;
            memcpy(file + oldLine, file + i + 2, *fileSize);
            i = oldLine;
        }
    }
    *programSize = programIndex -1;
    // Add addresses insted of labels
    for (int i = 0; i < *programSize; i++)
    {
        for(int j = 0; j < symbolIndex; j++)
        {
            if((match = re_match(gSymbolTable[j].label, program[i].instruction, &len)) != -1)
            {
                char buffer[10] = {0};
                int size = 0;
                int relAddress = 0;
                int labelLen = strlen(gSymbolTable[j].label);
                int oppcodeLen = 0;
                char oppcode[10] = {0};
                re_match(CASM_OPPCODE_RGX,program[i].instruction,&oppcodeLen);
                memcpy(oppcode, program[i].instruction, oppcodeLen);
                int type = getInstructionType(oppcode);
                if((type & CASM_B2_MASK) == CASM_B2_MASK)
                {
                    relAddress = gSymbolTable[i].address + START_ADDR;
                }
                else
                {
                    relAddress = gSymbolTable[j].address - program[i].address;
                    if (program[i].address > gSymbolTable[j].address)
                    {
                        relAddress = gSymbolTable[j].address - program[i].address;
                    }
                }
                getAsciiNumber(relAddress, buffer, &size);
                memcpy(program[i].instruction + match, buffer, size);
                program[i].instruction[match + size] = '\0';
                memcpy(program[i].label, gSymbolTable[j].label, len);
            }
        }
    }
}

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

void preprocessFile(char *file, casm_program_t* program, int* programSize)
{
    int size = strlen(file);

    removeComments(file,&size);

    trimWhiteSpaces(file,&size);

    procsToLabels(file,&size);

    labelsToAdresses(file, &size, program, programSize);
}
