#include "resource_type_classifier.h"
uint32_t sa_resource_type_classifier_8c64f206(uint32_t *sr,uint32_t type)
{
 uint32_t matches=type==0x500u||type==0x600u||type==0x700u||type==0x800u;
 *sr=(*sr&~1u)|matches;
 return matches;
}
