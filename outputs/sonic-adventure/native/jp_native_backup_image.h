#ifndef JP_NATIVE_BACKUP_IMAGE_H
#define JP_NATIVE_BACKUP_IMAGE_H
#include <stddef.h>
#include <stdint.h>
#undef NULL
#include <shinobi.h>
#define JP_BACKUP_API __declspec(dllexport)
/* Bounded native boundary over the original bu* image functions. The maximum
 * is the SDK's largest supported card, 1 MB. Sizes are bytes, result codes
 * retain the SDK's signed 32-bit representation. Build returns block count.
 * Caller supplies valid readable header/source spans and writable output.
 */
JP_BACKUP_API int32_t jp_backup_image_build(void *output,size_t capacity,
                                          const BUS_BACKUPFILEHEADER *header);
/* Analyze never writes the input image, including its CRC. As in the SDK,
 * CRC mismatch fills the parsed header before returning BUPFILE_CRC; illegal
 * or truncated images leave the header unchanged. Output pointers borrow the
 * caller's image and expire with it; they are not newly owned allocations.
 * Header output may not alias the input span. */
JP_BACKUP_API int32_t jp_backup_image_analyze(BUS_BACKUPFILEHEADER *header,
                                            const void *image,size_t bytes);
#endif
