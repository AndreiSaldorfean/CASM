#include "casm_preprocessor.h"
#include <stdio.h>
#include "re.h"
#include <string.h>
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

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

static void trimNewLines(char* file, int* fileSize)
{
    char buffer[500] = {0};
    int j = 0;
    int i = 0;

    // Trim the newlines
    for(i = 0; i < *fileSize; i++)
    {
        if(
            (file[i] == '\n' && file[i+1] == '\n') ||
            (file[i] == ' '  && file[i+1] == ' ')  ||
            (file[i] == ' ' && file[i+1] == '\n')
        )
        {
            continue;
        }
        buffer[j++] = file[i];
    }
    *fileSize = j;
    memcpy(file, buffer, j+1);
}

static void getAsciiNumber(int number, char* result, int* size) {
    int digit;
    char buf[4] = {0};
    int i = 0;
    int isSigned = 0;
    if (number == 0) {
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

static void removeComments(char* file, char* destination, int* fileSize)
{
    int i=0;
    int j=0;

    for(i=0; i < *fileSize; i++)
    {
        if (file[i] == ';')
        {
            while(file[i] != '\n')
            {
                i++;
            }
        }

        destination[j++] = file[i];
    }
    *fileSize = j;
}

static void procsToLabels(char *file, int* fileSize)
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
                *fileSize -= i - match;
                break;
            }
        }
    }
}

static void labelsToAdresses(char* file, int *fileSize)
{
    int len = 0;
    int match = 0;
    int match1 = 0;
    char labelBuffer[50] = {0};

    // Replace the labes with their addresses
    while(1)
    {
        int x = 0;
        int y = 0;
        match1 = re_match("\\S+:",file,&len);
        if(match1 == -1)
        {
            break;
        }
        x = match1;
        memcpy(labelBuffer, file + match1, len-1);
        memcpy(file + match1, file + match1 + len+1, *fileSize - match1);
        *fileSize -=len;
        int oldMatch = 0;
        while((match = re_match(labelBuffer, file + oldMatch, &len)) != -1)
        {
            int direction = 1;
            int ct=0;
            match += oldMatch;
            oldMatch = match + 1;
            y = x;

            if ((y - match) < 0)
            {
                int temp = y;
                y = match;
                match = temp;
                direction = -1;
                ct++;
            }

            while(match < y)
            {
                if(file[match] == '(' && isdigit(file[match-1]))
                {
                    ct++;
                }

                if(file[match] == '\n')
                {
                    ct++;
                }
                match++;
            }

            char asciiNum[4] = {0};
            int len0 = 0;
            getAsciiNumber(((ct)*direction),asciiNum,&len0);

            int diff = len - len0;
            // Insert number
            memcpy(file + (oldMatch - 1), asciiNum, len0);

            // Erase remaning label
            memcpy(file + (oldMatch + len0 - 1), file + (oldMatch + len0 -1 + diff), *fileSize - oldMatch);
            *fileSize -= diff;
        }
    }
}

/* ============================================================================================================
                                            Global functions
============================================================================================================ */

void preprocessFile(char *buffer, char* preprocessedFile)
{
    int size = strlen(buffer);

    removeComments(buffer,preprocessedFile,&size);

    trimNewLines(preprocessedFile,&size);

    procsToLabels(preprocessedFile,&size);

    labelsToAdresses(preprocessedFile,&size);
}
