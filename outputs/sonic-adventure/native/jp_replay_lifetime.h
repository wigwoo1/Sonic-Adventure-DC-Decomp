/* Authored callback-lifetime boundary. One game thread, one linked instance.
 * Scopes live on the invoking C stack and must leave in reverse entry order.
 * A task's destruction invalidates every active replay of that identity,
 * including scopes suspended by nested callbacks. No task is dereferenced.
 */
#ifndef JP_REPLAY_LIFETIME_H
#define JP_REPLAY_LIFETIME_H
struct jp_replay_scope {
    const void *task;
    struct jp_replay_scope *previous;
    int destroyed;
};
void jp_replay_enter(struct jp_replay_scope *scope, const void *task);
int jp_replay_leave(struct jp_replay_scope *scope);
void jp_replay_destroying(const void *task);
#endif
