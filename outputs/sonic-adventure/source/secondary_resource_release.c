/* Original 0x8c04b468..0x8c04b48b; helper bodies remain dependencies. */
#include "secondary_resource_release.h"
SA_STAGE_EXPORT void sa_secondary_resource_release_8c04b468(
 struct sa_secondary_resource_owner *owner,const struct sa_secondary_release_services *services) {
 if(owner->resource) {
  services->release_8c04b48c(services->context,owner);
  services->release_8c04b82e(services->context,owner);
  services->free_8c098b42(services->context,owner->resource);
  owner->resource=0;
 }
}
