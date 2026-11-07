/* This phase owns the ring list for the duration of one stage update. */
void jp_emerald_ring_collide(struct collision_work *ring);

void jp_collision_forget_ring(struct collision_work *ring)
{
    for (Uint16 i = 0; i < collision_ring_count; ++i)
        if (collision_rings[i] == ring) collision_rings[i] = NULL;
}

void jp_collision_finish_ring_pass(void)
{
    for (Uint16 i = 0; i < collision_ring_count; ++i) {
        struct collision_work *ring = collision_rings[i];
        collision_rings[i] = NULL;
        if (ring) jp_emerald_ring_collide(ring);
    }
    collision_ring_count = 0;
}

__declspec(dllexport) unsigned jp_collision_pending_rings(void)
{
    return collision_ring_count;
}
