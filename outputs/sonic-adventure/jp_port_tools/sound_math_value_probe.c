/* Result-bit callers; no arithmetic or game-service replacement. */
#include <stdint.h>
#include <string.h>
__declspec(dllimport) float njSin(int32_t);
__declspec(dllimport) float njCos(int32_t);
__declspec(dllimport) float njSqrt(float);
__declspec(dllimport) float vector_length_squared(float,float,float);
static uint32_t bits(float x){uint32_t word;memcpy(&word,&x,4);return word;}
__declspec(dllexport) uint32_t sound_probe_sin(int32_t angle){return bits(njSin(angle));}
__declspec(dllexport) uint32_t sound_probe_cos(int32_t angle){return bits(njCos(angle));}
__declspec(dllexport) uint32_t sound_probe_sqrt(uint32_t word){float x;memcpy(&x,&word,4);return bits(njSqrt(x));}
__declspec(dllexport) uint32_t sound_probe_length(const void *input)
{float v[3];memcpy(v,input,12);return bits(vector_length_squared(v[0],v[1],v[2]));}
