/* Authored synchronous Windows graphics backend for real owned frame IDs.
 * This is offscreen rendering/readback, not kmRender/flip or a playable loop.
 */
#ifndef JP_NATIVE_GPU_H
#define JP_NATIVE_GPU_H
#include "jp_native_packets.h"
struct jp_gpu;
enum jp_gpu_error { JP_GPU_OK,JP_GPU_ARGUMENT,JP_GPU_DEVICE,JP_GPU_SHADER,
    JP_GPU_PACKETS,JP_GPU_STATE,JP_GPU_TEXTURE,JP_GPU_BUDGET,JP_GPU_ALLOCATION,
    JP_GPU_API,JP_GPU_GEOMETRY };
struct jp_gpu_failure { int code; uint32_t batch; int32_t hresult; };
struct jp_gpu_config {
    uint32_t width,height;
    float logical_width,logical_height;
    uint32_t textures,flat_color_vertex;
    float clear_rgba[4],punch_alpha,cull_small_area;
    const uint32_t *palette;
    size_t palette_entries;
    uint32_t stride_pixels;
    size_t budget;
};
struct jp_gpu_report {
    size_t batches,triangles,draw_calls,texture_uploads,decoded_texels;
    size_t accounted_payload_bytes;
    uint32_t width,height,feature_level,textures_enabled;
    uint32_t shader_texture_bindings,shader_sampler_bindings;
};
JP_PACKET_API int jp_gpu_open(struct jp_gpu **out,struct jp_gpu_failure *failure);
JP_PACKET_API void jp_gpu_close(struct jp_gpu *gpu);
/* One game thread; frame/palette/config/output remain valid through return.
 * Output is tightly packed RGBA8. All spans and output/report storage must be
 * disjoint. Failure preserves pixels and report. Caller still releases frame.
 * Completed Map/readback waits for this call's GPU reads; there is no borrowed
 * resource after return. Texture Off does not fetch/decode/upload/sample any
 * texture; vertex/material base and offset colors remain.
 * Native floating color policy, explicit submission-order translucency, clear,
 * palette/stride and small-cull threshold. flat_color_vertex is 0 or 2 (explicit
 * native first/last choice, console varying-flat-color policy not qualified).
 * Not a substitute for missing actual frame register/state binding. Supported
 * states and remaining hardware fidelity gates are listed in the GPU guide.
 * Budget covers requested CPU/GPU payloads conservatively, not driver overhead.
 */
JP_PACKET_API int jp_gpu_draw_frame(struct jp_gpu *gpu,uint64_t frame,
    const struct jp_gpu_config *config,void *rgba,size_t bytes,
    struct jp_gpu_report *report,struct jp_gpu_failure *failure);
#endif
