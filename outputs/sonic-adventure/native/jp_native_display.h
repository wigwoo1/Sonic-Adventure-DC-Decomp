#ifndef JP_NATIVE_DISPLAY_H
#define JP_NATIVE_DISPLAY_H
#include <stdint.h>
#include <stddef.h>
enum jp_display_error {JP_DISPLAY_OK,JP_DISPLAY_ARGUMENT,JP_DISPLAY_UNAVAILABLE};
struct jp_display_info {
    uint32_t bytes,width,height,bits_per_pixel,reported_hz,hz_valid;
    uint32_t legacy_cable,display_flags;
};
/* Read-only current Windows display query. Dimensions are physical pixels;
 * the refresh value is driver-reported, not measured presentation timing.
 * legacy_cable=VGA identifies this port's progressive-PC compatibility path;
 * it does not identify the connector/cable on the user's physical monitor.
 * Failure preserves caller bytes and never changes desktop display settings. */
__declspec(dllexport) int jp_display_query(struct jp_display_info *,size_t bytes);
#endif
