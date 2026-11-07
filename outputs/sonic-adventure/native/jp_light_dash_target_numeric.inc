/* The ring selector only asks for an unsigned angle. */
uint32_t jp_rz_camera_acos(uint32_t);
static Angle light_dash_angle(const NJS_VECTOR *a, const NJS_VECTOR *b)
{
    float left[4] = {a->x, a->y, a->z, 0};
    float right[4] = {b->x, b->y, b->z, 0};
    uint32_t lhs[4], rhs[4];
    memcpy(lhs, left, sizeof lhs);
    memcpy(rhs, right, sizeof rhs);
    uint32_t word = sh4_fipr(lhs, rhs, 1);
    float dot;
    memcpy(&dot, &word, sizeof dot);
    if (dot > 1) word = 0x3f800000u;
    if (dot < -1) word = 0xbf800000u;
    word = jp_rz_camera_acos(word);
    if ((word & 0x7fffffffu) >= 0x7f800000u) return 0x8000;
    float radians;
    memcpy(&radians, &word, sizeof radians);
    return (Angle)(radians * ANGLE_PER_RADIAN);
}

static void light_dash_subtract(NJS_VECTOR *a, const NJS_VECTOR *b)
{
    a->x = jp_sprite_subtract(a->x, b->x);
    a->y = jp_sprite_subtract(a->y, b->y);
    a->z = jp_sprite_subtract(a->z, b->z);
}
