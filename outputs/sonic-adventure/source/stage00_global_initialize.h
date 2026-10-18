#ifndef SA_STAGE00_GLOBAL_INITIALIZE_H
#define SA_STAGE00_GLOBAL_INITIALIZE_H
#include <stdint.h>
#ifdef _WIN32
#define SA_STAGE00_GLOBAL_EXPORT __declspec(dllexport)
#else
#define SA_STAGE00_GLOBAL_EXPORT
#endif

/* Six words initialized by the STG00 helper at 0x0c900e00. */
struct sa_stage00_global_record {
 uint32_t word_00,word_04,word_08,word_0c,word_10,word_14;
};
SA_STAGE00_GLOBAL_EXPORT void sa_stage00_global_initialize_0c900e00(
 struct sa_stage00_global_record *record);
#endif
