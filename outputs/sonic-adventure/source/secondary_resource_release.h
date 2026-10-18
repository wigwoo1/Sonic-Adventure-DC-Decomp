#ifndef SA_SECONDARY_RESOURCE_RELEASE_H
#define SA_SECONDARY_RESOURCE_RELEASE_H
#include "stage00_task.h"
/* Native projection of the original work's resource pointer at offset 60. */
struct sa_secondary_resource_owner { void *resource; };
struct sa_secondary_release_services {
 void *context;
 void (*release_8c04b48c)(void *,struct sa_secondary_resource_owner *);
 void (*release_8c04b82e)(void *,struct sa_secondary_resource_owner *);
 void (*free_8c098b42)(void *,void *);
};
SA_STAGE_EXPORT void sa_secondary_resource_release_8c04b468(
 struct sa_secondary_resource_owner *,const struct sa_secondary_release_services *);
#endif
