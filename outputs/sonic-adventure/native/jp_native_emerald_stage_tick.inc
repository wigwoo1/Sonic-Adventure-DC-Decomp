#include "player/player.h"

void jp_emerald_placement_viewers(struct task_work *, struct task_work *, struct task_work *, int);
int jp_emerald_placement_step(struct task_work *, struct task_work *);

/* Run once after the player's simulation step, before rendering. The
 * caller owns the Sonic task and camera for this call. */
__declspec(dllexport) int jp_emerald_stage_tick(struct task *sonic, struct task_work *camera)
{
    if (!(stage_session & STAGE_FLOOR) || emerald_stage_stepping ||
        !sonic || !sonic->work || !sonic->motion || !sonic->motion->record)
        return -1;
    struct task_work *work = sonic->work;
    struct collision_work *collision = work->collision;
    if (!collision || !collision->count || !collision->shapes)
        return -1;
    struct collision_shape *shape = collision->shapes;
    if ((shape->flags & (SHAPE_KIND | SHAPE_IN_WORLD)) != (SHAPE_SPHERE | SHAPE_IN_WORLD) ||
        !(shape->radius > 0 && shape->radius <= 1000.0F))
        return -1;

    emerald_stage_stepping = 1;
    jp_emerald_placement_viewers(work, NULL, camera, 0);
    jp_emerald_ring_collector(&shape->center, shape->radius,
                             sonic->motion->record->effects & PLAYER_FALLING, NULL);
    int result = jp_emerald_placement_step(work, camera);
    if (result >= 0) task_step_level(3);
    jp_emerald_ring_collector(NULL, 0, 0, NULL);
    jp_emerald_placement_viewers(NULL, NULL, NULL, 0);
    emerald_stage_stepping = 0;
    return result;
}
