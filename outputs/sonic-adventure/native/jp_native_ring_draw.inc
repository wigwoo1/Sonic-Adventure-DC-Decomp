#include <stdint.h>

struct simple_mesh_span { size_t indices, uvs; };
int jp_simple_draw_model_bounded(NJS_MODEL *, size_t, size_t, size_t,
                                const struct simple_mesh_span *, size_t, int32_t *);
extern NJS_TEXLIST *_nj_current_texlist;

static NJS_MODEL *ring_draw_model;
static const struct simple_mesh_span *ring_draw_span;
static NJS_TEXLIST *ring_draw_textures;
static NJS_POINT3 ring_draw_rebase;
static int ring_draw_count, ring_draw_error;

static void native_ring_draw(struct task *task)
{
    struct task_work *work = task->work;
    if (!ring_draw_model || ring_draw_error || work->mode != RING_WAIT) return;
    if (!njPushMatrix(NULL)) {
        ring_draw_error = 1;
        return;
    }
    NJS_TEXLIST *saved = _nj_current_texlist;
    if (njSetTexture(ring_draw_textures) != 1) {
        _nj_current_texlist = saved;
        njPopMatrix(1);
        ring_draw_error = 1;
        return;
    }
    njTranslateV(NULL, &ring_draw_rebase);
    njTranslateV(NULL, &work->pos);
    njRotateY(NULL, work->ang.y);
    int32_t projection;
    ring_draw_error = jp_simple_draw_model_bounded(ring_draw_model,
        ring_draw_model->nbPoint, ring_draw_model->nbPoint, 1, ring_draw_span, 1, &projection);
    _nj_current_texlist = saved;
    njPopMatrix(1);
    if (!ring_draw_error) ++ring_draw_count;
}

/* The caller owns the model and texture list through the scheduler pass.
 * The frame retains any texture surfaces it references afterwards. */
__declspec(dllexport) int jp_emerald_ring_draw_level(NJS_MODEL *model,
    const struct simple_mesh_span *span, NJS_TEXLIST *textures, const NJS_POINT3 *rebase)
{
    if (ring_draw_model || !model || !span || !textures || !rebase) return -1;
    ring_draw_count = ring_draw_error = 0;
    ring_draw_model = model;
    ring_draw_span = span;
    ring_draw_textures = textures;
    ring_draw_rebase = *rebase;
    task_draw_level(3);
    ring_draw_model = NULL;
    ring_draw_span = NULL;
    ring_draw_textures = NULL;
    return ring_draw_error ? -1 : ring_draw_count;
}
