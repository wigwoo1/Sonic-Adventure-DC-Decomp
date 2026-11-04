/* Authored platform-neutral consumer of owned Kamui polygon-list bytes.
 * No SDK/game source bodies, graphics API or retail assets are included.
 * Immutable input spans must remain accessible for the complete decode call.
 */
#ifndef JP_NATIVE_PACKETS_H
#define JP_NATIVE_PACKETS_H
#include <stddef.h>
#include <stdint.h>
#if defined(_WIN32)
#define JP_PACKET_API __declspec(dllexport)
#else
#define JP_PACKET_API
#endif
enum jp_packet_error { JP_PACKET_OK, JP_PACKET_ARGUMENT, JP_PACKET_TRUNCATED,
    JP_PACKET_COMMAND, JP_PACKET_SEQUENCE, JP_PACKET_UNSUPPORTED,
    JP_PACKET_NONFINITE, JP_PACKET_BUDGET, JP_PACKET_ALLOCATION,
    JP_PACKET_OVERFLOW, JP_PACKET_FRAME };
struct jp_packet_span { const void *data; size_t bytes; };
struct jp_packet_failure { int code; unsigned list; size_t offset; };
/* Colors are RGBA, without device quantization/clamping. For intensity types,
 * base/offset contain the face colors and the intensities stay separate.
 * Intensity applies to RGB; alpha is the face alpha. No color information is
 * discarded to choose an unverified hardware rounding policy here.
 */
struct jp_packet_vertex {
    float x,y,inv_w,u,v,base[4],offset[4],base_intensity,offset_intensity;
};
struct jp_packet_triangle { size_t indices[3]; };
struct jp_packet_batch {
    uint32_t header[16];
    unsigned header_words,list,vertex_type;
    size_t source_offset,first_vertex,vertex_count,first_triangle,triangle_count;
};
struct jp_packet_plan {
    size_t allocation_bytes,batch_count,vertex_count,triangle_count;
    const struct jp_packet_batch *batches;
    const struct jp_packet_vertex *vertices;
    const struct jp_packet_triangle *triangles;
};
/* Two-pass, bounded allocation. On any failure *out is unchanged. Failures
 * identify a list and byte offset. Empty lists/frames are valid. No EOL packet
 * is fabricated. Each nonempty input is exactly the committed queue span.
 * Header metadata is retained for later depth/blend/fog/etc interpretation.
 * Modifier, hardware-sprite, two-volume and tile-clip commands are explicitly
 * unsupported, never skipped. Ordinary vertex layouts 0..8 are supported.
 * Previous-face color needs an earlier explicit face header in the same list.
 */
JP_PACKET_API int jp_packets_decode(const struct jp_packet_span lists[5],
    size_t budget,struct jp_packet_plan **out,struct jp_packet_failure *failure);
JP_PACKET_API void jp_packets_destroy(struct jp_packet_plan *plan);
/* Compiled separately against the real frame owner. The plan owns decoded
 * arrays, but does not retain or decode texture payloads. A textured GPU
 * consumer must keep the original frame alive until uploads have completed.
 */
JP_PACKET_API int jp_packets_decode_frame(uint64_t frame,size_t budget,
    struct jp_packet_plan **out,struct jp_packet_failure *failure);
#endif
