#include "stage00_task.h"
/* Exact integer register effects of the complete 8c055d4e..8c055d5a body. */
SA_STAGE_EXPORT uint32_t sa_model_channel_difference_8c055d4e(uint32_t *registers){
 uint32_t difference=(registers[5]&65535u)-(registers[4]&65535u);
 registers[3]=difference;registers[4]=difference;
 registers[0]=(uint32_t)(int32_t)(int16_t)difference;return registers[0];
}
