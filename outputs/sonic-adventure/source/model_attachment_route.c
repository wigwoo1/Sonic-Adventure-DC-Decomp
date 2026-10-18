#include "stage00_task.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
/* Rendering record types 4..9: two fixed entity attachment destinations,
 * then original matrix-stack service. Transform implementations pending. */
SA_STAGE_EXPORT int sa_model_attachment_route(uint32_t type,uint32_t entity,uint32_t frame,uint32_t *sr,uint32_t incoming,void *context,sa_vector_service service,uint32_t *result){
 if(type<4u||type>9u)return -1;
 uint32_t offset=(0x53u+(type-4u)*3u)*4u;
 uint32_t r0=service(context,0x8c638eccu,3u,0u,frame+136u,entity+offset,incoming,sr);
 r0=service(context,0x8c638f28u,3u,0u,frame+124u,entity+offset+72u,r0,sr);
 *result=service(context,0x8c639b98u,1u,1u,0u,0u,r0,sr);return 0;
}
