extern NJS_VIEW *_nj_view_;
extern NJS_MATRIX _nj_view_matrix_;
extern NJS_MATRIX *_nj_base_matrix_ptr_;
extern Sint32 _stack_size_;

/* Build the unobstructed POINT pose. The original collision push-out,
 * smoothing, trigger orbit and gravity roll are separate camera phases. */
__declspec(dllexport) int jp_emerald_point_view(const NJS_POINT3 *player, NJS_MATRIX *out)
{
    struct emerald_camera_mode mode;
    if (!player || !out || jp_emerald_camera_mode(&mode) ||
        mode.mode < POINT_MODE_FIRST || mode.mode > POINT_MODE_LAST ||
        !(mode.distance > 0)) return -1;
    NJS_VECTOR direction = {player->x-mode.target.x, player->y-mode.target.y,
                            player->z-mode.target.z};
    if (direction.x == 0 && direction.y == 0 && direction.z == 0) return -1;
    njUnitVector(&direction);
    NJS_VIEW view = {0};
    view.px = player->x + mode.distance*direction.x;
    view.py = player->y + 7.5F + mode.distance*direction.y;
    view.pz = player->z + mode.distance*direction.z;
    view.vx = -direction.x;
    view.vy = -direction.y;
    view.vz = -direction.z;

    NJS_VIEW *saved_view = _nj_view_;
    NJS_MATRIX saved, current, scratch;
    NJS_MATRIX *base = _nj_base_matrix_ptr_, *top = _nj_current_matrix_ptr_;
    Sint32 depth = _stack_size_, flag = _nj_matrix_flag_;
    memcpy(saved, _nj_view_matrix_, sizeof saved);
    njGetMatrix(&current);
    _nj_base_matrix_ptr_ = _nj_current_matrix_ptr_ = &scratch;
    _stack_size_ = 1;
    njSetView(&view);
    memcpy(out, _nj_view_matrix_, sizeof *out);
    _nj_base_matrix_ptr_ = base;
    _nj_current_matrix_ptr_ = top;
    _stack_size_ = depth;
    njSetMatrix(NULL, &current);
    _nj_matrix_flag_ = flag;
    memcpy(_nj_view_matrix_, saved, sizeof saved);
    _nj_view_ = saved_view;
    return 0;
}
