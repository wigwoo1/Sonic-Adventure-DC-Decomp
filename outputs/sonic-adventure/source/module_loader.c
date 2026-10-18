/* Recovered USA Rev A module-loader wrapper, 0x8c09da44..0x8c09da73.
 * A negative file result skips decompression. Post-load synchronization always
 * occurs, and the wrapper returns the file result rather than decoded length.
 * External file-I/O and synchronization routines remain backend dependencies.
 */
#include "module_loader.h"
uint32_t sa_prs_decompress_8c04c5e0(const uint8_t *input,uint8_t *output);
SA_MODULE_EXPORT int32_t sa_load_compressed_module_8c09da44(const char *name,uint8_t *scratch,
 uint8_t *destination,const struct sa_module_services *services) {
 int32_t result=services->read_file(services->context,name,scratch);
 if(result>=0)sa_prs_decompress_8c04c5e0(scratch,destination);
 services->finish_load(services->context);
 return result;
}
