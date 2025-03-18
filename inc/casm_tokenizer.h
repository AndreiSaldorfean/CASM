#ifndef TOKENIZER_H
#define TOKENIZER_H

#ifdef __cplusplus
extern "C"{
#endif

/* ============================================================================================================
                                            DEFINES
============================================================================================================ */

char* tokenizeInstructionTypeB1(const char* inst);
char* tokenizeInstructionTypeB2(const char* inst);
char* tokenizeInstructionTypeB3(const char* inst);
char* tokenizeInstructionTypeB4(const char* inst);

#ifdef __cplusplus
}
#endif
#endif /* TOKENIZER_H */