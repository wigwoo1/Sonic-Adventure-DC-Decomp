/* Thin native callers retain float result bits across the Python FFI. */
#include <stdint.h>
#include <string.h>
__declspec(dllimport) float njUnitVector(void *);
__declspec(dllimport) float jp_qsound_position_side(float);
__declspec(dllimport) int32_t jp_qsound_position_channel(float);
__declspec(dllexport) uint32_t spatial_probe_unit_bits(void *vector)
{float result=njUnitVector(vector);uint32_t word;memcpy(&word,&result,4);return word;}
__declspec(dllexport) uint32_t spatial_probe_side_bits(uint32_t word)
{float input;memcpy(&input,&word,4);float result=jp_qsound_position_side(input);memcpy(&word,&result,4);return word;}
__declspec(dllexport) int32_t spatial_probe_channel_bits(uint32_t word)
{float input;memcpy(&input,&word,4);return jp_qsound_position_channel(input);}
