/* Original 0x8c0344a0..0x8c0344db. Retains the initial resource pointer
 * across frees, then clears its owner's resource field. */
#include "work_resource_release.h"
SA_STAGE_EXPORT void sa_work_resource_release_8c0344a0(struct sa_resource_owner *owner,
 void *context,void (*free_resource)(void *,void *)) {
 struct sa_work_resource *resource=owner->resource;
 if(resource) {
  if((resource->flags & 0x20u) && resource->child) {
   free_resource(context,resource->child);
   resource->child=0;
  }
  free_resource(context,resource);
  owner->resource=0;
 }
}
