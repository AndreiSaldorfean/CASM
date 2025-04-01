#include "unity.h"
#include "ts_preprocessor.h"
#include "casm_preprocessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/* ============================================================================================================
                                            DEFINES and ENUMS
============================================================================================================ */

/* ============================================================================================================
                                            TYPEDEFS AND STRUCTS
============================================================================================================ */

/* ============================================================================================================
                                            Local Variables
============================================================================================================ */
static char* expectedResult1 = "mov r1, r2\nmov r2, r1\n";
static char* expectedResult2[200] =
{
    "label1:\nmov r1, 1\nret\nlabel2:\nmov r1, 1\nret\nlabel3:\nmov r1, 1\nret\n",
    "first_label1:\nmov r1, 1\nret\nsecond_label2:\nmov r1, 1\nret\nThird_label3:\nmov r1, 1\nret\n",
};
static casm_program_t expectedResult3[200] =
{
    {
        .instruction = "jmp 6",
        .address = 0
    },
    {
        .instruction = "mov [r1],[r2]",
        .address = 2
    },
    {
        .instruction = "ret",
        .address = 4
    },
    {
        .instruction = "jmp 4",
        .address = 6
    },
    {
        .instruction = "ret",
        .address = 8
    },
    {
        .instruction = "jmp -10",
        .address = 10
    },
    {
        .instruction = "ret",
        .address = 12
    },
};
/* ============================================================================================================
                                            Global Variables
============================================================================================================ */

/* ============================================================================================================
                                            Local functions
============================================================================================================ */

/* ============================================================================================================
                                            Global functions
============================================================================================================ */
extern void trimWhiteSpaces(char* file, int* fileSize);
extern void getAsciiNumber(int number, char* result, int* size);
extern void removeComments(char* file,int* fileSize);
extern void procsToLabels(char *file, int* fileSize);
extern void labelsToAdresses(char* file, int *fileSize, casm_program_t* program, int* programSize);

/**
 * @brief Test trim white spaces function
 */
void TS_Preprocessor0(void)
{
    const char *dir_path = "tests/resources/ds_remove_whitespaces";
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    FILE* fp = NULL;
    int size = 0;
    char fileContents[500] = {0};
    int dir_path_len = strlen(dir_path);
    char filePath[100] = {0};

    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        memset(fileContents,0,500);
        memset(filePath,0,100);

        memcpy(filePath,dir_path, dir_path_len);
        filePath[dir_path_len] = '/';
        memcpy(filePath+dir_path_len+1, entry->d_name, strlen(entry->d_name));

        fp = fopen(filePath,"rb");

        fread(fileContents,1,500,fp);

        size = strlen(fileContents);
        trimWhiteSpaces(fileContents,&size);
        for(int i = 0; i < size; i++)
        {
            TEST_ASSERT_EQUAL(expectedResult1[i],fileContents[i]);
        }

        fclose(fp);
    }

    closedir(dir);
}

/**
 * @brief Test get ascii num function
 */
void TS_Preprocessor1(void)
{
    int number = 0;
    char result[6] = {0};
    int size = 0;

    {
        memset(result,0,6);
        number = 123;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("123",result);
        TEST_ASSERT_EQUAL_INT(3,size);
    }

    {
        memset(result,0,6);
        number = 1;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("1",result);
        TEST_ASSERT_EQUAL_INT(1,size);
    }

    {
        memset(result,0,6);
        number = -123;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("-123",result);
        TEST_ASSERT_EQUAL_INT(4,size);
    }

    {
        memset(result,0,6);
        number = 0;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("0",result);
        TEST_ASSERT_EQUAL_INT(1,size);
    }

    {
        memset(result,0,6);
        number = 127;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("127",result);
        TEST_ASSERT_EQUAL_INT(3,size);
    }

    /* Number too large */
    {
        memset(result,0,6);
        number = 129;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("",result);
        TEST_ASSERT_EQUAL_INT(0,size);
    }

    /* Number too small */
    {
        memset(result,0,6);
        number = -128;
        getAsciiNumber(number,result,&size);

        TEST_ASSERT_EQUAL_STRING("",result);
        TEST_ASSERT_EQUAL_INT(0,size);
    }
}

/**
 * @brief Test remove comments function
 */
void TS_Preprocessor2(void)
{
    const char *dir_path = "tests/resources/ds_remove_comments";
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    FILE* fp = NULL;
    int size = 0;
    char fileContents[500] = {0};
    int dir_path_len = strlen(dir_path);
    char filePath[100] = {0};
    char destination[500] = {0};

    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        memset(fileContents,0,500);
        memset(filePath,0,100);

        memcpy(filePath,dir_path, dir_path_len);
        filePath[dir_path_len] = '/';
        memcpy(filePath+dir_path_len+1, entry->d_name, strlen(entry->d_name));

        fp = fopen(filePath,"rb");

        fread(fileContents,1,500,fp);

        size = strlen(fileContents);
        removeComments(fileContents,&size);
        for(int i = 0; i < size; i++)
        {
            TEST_ASSERT_EQUAL(expectedResult1[i],fileContents[i]);
        }

        fclose(fp);
    }

    closedir(dir);
}

/**
 * @brief Test procs to labels function
 */
void TS_Preprocessor3(void)
{
    const char *dir_path = "tests/resources/ds_procs_to_labels";
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    FILE* fp = NULL;
    int size = 0;
    char fileContents[500] = {0};
    int dir_path_len = strlen(dir_path);
    char filePath[100] = {0};
    char destination[500] = {0};
    int j = 0;

    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        memset(fileContents,0,500);
        memset(filePath,0,100);

        memcpy(filePath,dir_path, dir_path_len);
        filePath[dir_path_len] = '/';
        memcpy(filePath+dir_path_len+1, entry->d_name, strlen(entry->d_name));

        fp = fopen(filePath,"rb");

        fread(fileContents,1,500,fp);

        size = strlen(fileContents);
        if (size == 0)
        {
            continue;
        }

        procsToLabels(fileContents,&size);
        for(int i = 0; i < size; i++)
        {
            TEST_ASSERT_EQUAL(expectedResult2[j][i],fileContents[i]);
        }
        j++;
        fclose(fp);
    }

    closedir(dir);
}

/**
 * @brief Test label to address function
 */
void TS_Preprocessor4(void)
{
    const char *dir_path = "tests/resources/ds_label_to_addr";
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    FILE* fp = NULL;
    int size = 0;
    char fileContents[500] = {0};
    int dir_path_len = strlen(dir_path);
    char filePath[100] = {0};
    char destination[500] = {0};
    int j = 0;
    casm_program_t program[200] = {0};

    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        memset(fileContents,0,500);
        memset(filePath,0,100);

        memcpy(filePath,dir_path, dir_path_len);
        filePath[dir_path_len] = '/';
        memcpy(filePath+dir_path_len+1, entry->d_name, strlen(entry->d_name));

        fp = fopen(filePath,"rb");

        fread(fileContents,1,500,fp);

        size = strlen(fileContents);
        if (size == 0)
        {
            continue;
        }

        int programSize = 0;
        labelsToAdresses(fileContents,&size,program, &programSize);
        for(int i = 0; i < programSize; i++)
        {
            for(int j=0; expectedResult3[i].instruction[j]; j++)
            {
                TEST_ASSERT_EQUAL(expectedResult3[i].instruction[j],program[i].instruction[j]);
            }
            TEST_ASSERT_EQUAL(expectedResult3[i].address, program[i].address);
        }
        j++;
        fclose(fp);
    }

    closedir(dir);
}

/**
 * @brief Test preprocess function
 */
void TS_Preprocessor5(void)
{
    const char *dir_path = "tests/resources/ds_preprocessor";
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    FILE* fp = NULL;
    int size = 0;
    char fileContents[500] = {0};
    int dir_path_len = strlen(dir_path);
    char filePath[100] = {0};
    char destination[500] = {0};
    int j = 0;
    int programSize = 0;
    casm_program_t program[200] = {0};

    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        memset(fileContents,0,500);
        memset(filePath,0,100);

        memcpy(filePath,dir_path, dir_path_len);
        filePath[dir_path_len] = '/';
        memcpy(filePath+dir_path_len+1, entry->d_name, strlen(entry->d_name));

        fp = fopen(filePath,"rb");

        fread(fileContents,1,500,fp);

        size = strlen(fileContents);
        if (size == 0)
        {
            continue;
        }

        preprocessFile(fileContents, program, &programSize);
        for(int i = 0; i < programSize; i++)
        {
            for(int j=0; j < strlen(expectedResult3[i].instruction); j++)
            {
                TEST_ASSERT_EQUAL(expectedResult3[i].instruction[j],program[i].instruction[j]);
            }
            TEST_ASSERT_EQUAL(expectedResult3[i].address, program[i].address);
        }
        j++;
        fclose(fp);
    }

    closedir(dir);
}