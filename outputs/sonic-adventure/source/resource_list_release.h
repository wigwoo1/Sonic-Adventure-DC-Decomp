#ifndef SA_RESOURCE_LIST_RELEASE_H
#define SA_RESOURCE_LIST_RELEASE_H
#include "fpu_cleanup_transfer.h"
/* Native arena offsets preserve retail field widths. Owner holds resource
 * offset at +60, resource list head at +76, node next at +12. Zero is null.
 * Owner/resource/nodes must occupy valid arena storage; lists are acyclic. */
SA_STAGE_EXPORT int sa_resource_list_release_8c04b82e(uint8_t *arena,uint32_t owner,
 struct sa_fpu_banks *,void *context,void (*free_node)(void *,uint32_t offset));
/* Retains initial resource; list head +64, node next +16, extra allocation +68. */
SA_STAGE_EXPORT int sa_resource_list_release_8c04b48c(uint8_t *arena,uint32_t owner,
 struct sa_fpu_banks *,void *context,void (*free_node)(void *,uint32_t offset));
/* Connect the recovered 8c04b468 wrapper to both arena-based helper bodies. */
SA_STAGE_EXPORT int sa_secondary_resource_release_in_arena(uint8_t *arena,uint32_t owner,
 struct sa_fpu_banks *,void *context,void (*free_node)(void *,uint32_t offset));
#endif
