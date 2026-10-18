#ifndef SA_MODULE_LOADER_H
#define SA_MODULE_LOADER_H
#include <stdint.h>
#ifdef _WIN32
#define SA_MODULE_EXPORT __declspec(dllexport)
#else
#define SA_MODULE_EXPORT
#endif
/* Native services for the two external operations called by the retail loader.
 * The backend must provide sufficient scratch/destination storage and implement
 * the original post-load synchronization semantics for its platform.
 */
struct sa_module_services {
 void *context;
 int32_t (*read_file)(void *context,const char *name,uint8_t *scratch);
 void (*finish_load)(void *context);
};
SA_MODULE_EXPORT int32_t sa_load_compressed_module_8c09da44(const char *name,uint8_t *scratch,
 uint8_t *destination,const struct sa_module_services *services);
#endif
