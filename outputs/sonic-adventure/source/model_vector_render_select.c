#include "stage00_task.h"
/* Original record-type dispatch 8c07a93c..8c07a986. Rendering bodies follow. */
SA_STAGE_EXPORT uint32_t sa_model_vector_render_select(uint32_t type,uint32_t *sr,uint32_t *r0){
 static const uint32_t targets[9]={0x8c07a986u,0x8c07ab40u,0x8c07ac0eu,0x8c07acdeu,0x8c07ad00u,0x8c07ad22u,0x8c07ad80u,0x8c07ada2u,0x8c07adc4u};
 type&=255u;*r0=(uint32_t)(int32_t)(int8_t)type;
 *sr=(*sr&~1u)|(type>=1u&&type<=9u);
 return type>=1u&&type<=9u?targets[type-1u]:0x8c07ade6u;
}
