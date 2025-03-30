#include "unity.h"
#include "ts_tokenizer.h"
#include "casm_tokenizer.h"

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

/* ============================================================================================================
                                            Global functions
============================================================================================================ */
/**
 * @brief Test preprocess function
 */
void TS_Tokenizer1(void)
{
    char buffer[3][20] = {0};
    char* instructionTokens[3] = {buffer[0],buffer[1],buffer[2]};
    tokenizeInstructionTypeB1("MOV   R1,   R2  ", instructionTokens);
    TEST_ASSERT_EQUAL_STRING("MOV", instructionTokens[0]);
    TEST_ASSERT_EQUAL_STRING("R1", instructionTokens[1]);
    TEST_ASSERT_EQUAL_STRING("R2", instructionTokens[2]);
}

/**
 * @brief Test preprocess function
 */
void TS_Tokenizer2(void)
{
    char buffer[3][20] = {0};
    char* instructionTokens[3] = {buffer[0],buffer[1],buffer[2]};

    tokenizeInstructionTypeB2("DEC   R1 ", instructionTokens);
    TEST_ASSERT_EQUAL_STRING("DEC", instructionTokens[0]);
    TEST_ASSERT_EQUAL_STRING("R1", instructionTokens[1]);
}

/**
 * @brief Test preprocess function
 */
void TS_Tokenizer3(void)
{
    char buffer[3][20] = {0};
    char* instructionTokens[3] = {buffer[0],buffer[1],buffer[2]};

    tokenizeInstructionTypeB2("BR   10 ", instructionTokens);
    TEST_ASSERT_EQUAL_STRING("BR", instructionTokens[0]);
    TEST_ASSERT_EQUAL_STRING("10", instructionTokens[1]);
}

/**
 * @brief Test preprocess function
 */
void TS_Tokenizer4(void)
{
    char buffer[3][20] = {0};
    char* instructionTokens[3] = {buffer[0],buffer[1],buffer[2]};

    tokenizeInstructionTypeB4("   NOP   ", instructionTokens);
    TEST_ASSERT_EQUAL_STRING("NOP", instructionTokens[0]);
}