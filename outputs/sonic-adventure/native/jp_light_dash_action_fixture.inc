__declspec(dllexport) int jp_emerald_light_dash_actions_check(NJS_ACTION *dash, NJS_ACTION *skid)
{
    struct player_action actions[ACTION_LIGHT_DASH + 1] = {0};
    actions[ACTION_STAND].nodes = 99;
    if (jp_emerald_bind_light_dash_actions(actions, ACTION_LIGHT_DASH, dash, skid) != -1 ||
        actions[ACTION_SKID].action || actions[ACTION_LIGHT_DASH].action) return 1;
    if (jp_emerald_bind_light_dash_actions(actions, ACTION_LIGHT_DASH + 1, dash, skid))
        return 2;
    const struct player_action *d = &actions[ACTION_LIGHT_DASH];
    const struct player_action *s = &actions[ACTION_SKID];
    if (d->action != dash || d->nodes != 73 || d->mode != PLAYER_ANIM_LOOP ||
        d->next != ACTION_SLIDE || d->blend != 1 || d->speed != 1) return 3;
    if (s->action != skid || s->nodes != 73 || s->mode != PLAYER_ANIM_ONCE ||
        s->next != ACTION_STAND || s->blend != 0.25F || s->speed != 1) return 4;
    return actions[ACTION_STAND].nodes != 99;
}
