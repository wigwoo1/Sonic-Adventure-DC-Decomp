#include "jp_replay_lifetime.h"
static struct jp_replay_scope *active;

void jp_replay_enter(struct jp_replay_scope *scope, const void *task)
{
    scope->task = task;
    scope->previous = active;
    scope->destroyed = 0;
    active = scope;
}

int jp_replay_leave(struct jp_replay_scope *scope)
{
    /* All callers are generated paired boundaries around a synchronous draw. */
    active = scope->previous;
    return !scope->destroyed;
}

void jp_replay_destroying(const void *task)
{
    for (struct jp_replay_scope *scope = active; scope; scope = scope->previous) {
        if (scope->task == task) scope->destroyed = 1;
    }
}
