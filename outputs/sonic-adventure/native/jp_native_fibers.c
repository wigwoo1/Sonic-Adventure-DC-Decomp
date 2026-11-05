#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_fibers.h"

enum { RF_SERVERS=8, RF_SLOTS=32 };
enum { FREE, READY, KILLED, ENDED };
struct native_fiber { void *context; void (*entry)(void); uint32_t wake;
    int state, indefinite; };
struct rf_server { int used, capacity, count, executing; uint32_t tick;
    void *scheduler; struct native_fiber slots[RF_SLOTS], *running; };
static struct rf_server servers[RF_SERVERS], *current;
static DWORD owner;
static void *root_fiber;
static int converted, error;
static DWORD os_error;
static uint64_t switches, passes;

static void fail(int code) { if (!error) { error=code; os_error=GetLastError(); } }
static int on_owner(void) { return owner && owner==GetCurrentThreadId(); }
static int valid(struct rf_server *s) {
    for (int i=0;i<RF_SERVERS;i++) if (s==servers+i) return s->used;
    return 0;
}
static int access_server(struct rf_server *s) {
    if (!on_owner()) return 0;
    if (!valid(s)) { fail(JP_RF_ARGUMENT); return 0; }
    return 1;
}
static void release(struct rf_server *s,struct native_fiber *f) {
    DeleteFiber(f->context); memset(f,0,sizeof(*f)); s->count--;
}
static VOID WINAPI enter(LPVOID value) {
    struct native_fiber *f=value;
    f->entry();
    /* Original entries end explicitly. Treat a native return as rfEndFiber
     * rather than allowing Windows to terminate the entire owning thread. */
    rfEndFiber();
}
int jp_rf_status(void) { return owner && !on_owner() ? JP_RF_THREAD : error; }
void jp_rf_clear_error(void) { if (!owner || on_owner()) { error=0; os_error=0; } }
int jp_rf_snapshot(struct jp_rf_info *out) {
    if (owner && !on_owner()) return JP_RF_THREAD;
    if (!out) return JP_RF_ARGUMENT;
    memset(out,0,sizeof(*out)); out->switches=switches; out->passes=passes;
    out->status=error; out->os_error=os_error;
    for (int i=0;i<RF_SERVERS;i++) if (servers[i].used) {
        out->servers++; out->fibers+=(uint32_t)servers[i].count;
    }
    return 0;
}
void rfInitSystem(long unused) {
    (void)unused;
    if (owner) { if (on_owner()) fail(JP_RF_STATE); return; }
    error=0; os_error=0;
    owner=GetCurrentThreadId(); converted=!IsThreadAFiber();
    root_fiber=converted ? ConvertThreadToFiberEx(NULL,FIBER_FLAG_FLOAT_SWITCH)
                         : GetCurrentFiber();
    if (!root_fiber) { fail(JP_RF_SYSTEM); owner=0; return; }
    memset(servers,0,sizeof(servers)); current=NULL; switches=passes=0;
}
struct rf_server *rfCreateServer(long capacity) {
    if (!on_owner()) return NULL;
    if (capacity<0 || capacity>RF_SLOTS) { fail(JP_RF_ARGUMENT); return NULL; }
    for (int i=0;i<RF_SERVERS;i++) if (!servers[i].used) {
        struct rf_server *s=servers+i; memset(s,0,sizeof(*s));
        s->used=1; s->capacity=(int)capacity; return s;
    }
    fail(JP_RF_CAPACITY); return NULL;
}
long rfCreateFiber(struct rf_server *s,void (*entry)(void),void *stack,unsigned long size) {
    if (!access_server(s)) return -1;
    if (!entry || !stack || size<4) { fail(JP_RF_ARGUMENT); return -1; }
    if (s->count==s->capacity) { fail(JP_RF_CAPACITY); return -1; }
    for (int i=0;i<s->capacity;i++) if (!s->slots[i].state) {
        struct native_fiber *f=s->slots+i;
        f->entry=entry; f->wake=s->tick;
        f->context=CreateFiberEx(16384,1024*1024,FIBER_FLAG_FLOAT_SWITCH,enter,f);
        if (!f->context) { memset(f,0,sizeof(*f)); fail(JP_RF_SYSTEM); return -1; }
        f->state=READY; s->count++; return i;
    }
    fail(JP_RF_STATE); return -1;
}
void rfDestroyServer(struct rf_server *s) {
    if (!access_server(s)) return;
    if (s->executing) { fail(JP_RF_STATE); return; }
    for (int i=0;i<s->capacity;i++) if (s->slots[i].state) release(s,s->slots+i);
    memset(s,0,sizeof(*s));
}
void rfExitSystem(void) {
    if (!on_owner()) return;
    if (current || GetCurrentFiber()!=root_fiber) { fail(JP_RF_STATE); return; }
    for (int i=0;i<RF_SERVERS;i++) if (servers[i].used) rfDestroyServer(servers+i);
    if (converted && !ConvertFiberToThread()) { fail(JP_RF_SYSTEM); return; }
    owner=0; root_fiber=NULL; converted=0;
}
void rfWakeupFiber(struct rf_server *s,long slot) {
    if (!access_server(s)) return;
    if (slot<0 || slot>=s->capacity) { fail(JP_RF_ARGUMENT); return; }
    struct native_fiber *f=s->slots+slot;
    if (f->state) { f->wake=s->tick; f->indefinite=0; }
}
long rfKillFiber(struct rf_server *s,long slot) {
    if (!access_server(s)) return -1;
    if (slot<0 || slot>=s->capacity) { fail(JP_RF_ARGUMENT); return -1; }
    struct native_fiber *f=s->slots+slot;
    if (f->state!=READY) return -1;
    rfWakeupFiber(s,slot); f->state=KILLED; return 0;
}
void rfKillFiberLevel(struct rf_server *s,long level) {
    if (!access_server(s)) return;
    /* No public setter exists; every original fiber starts at level zero. */
    if (level>=0) for (int i=0;i<s->capacity;i++) (void)rfKillFiber(s,i);
}
void rfSleepFiber(long ticks) {
    if (!on_owner() || !current || !current->running) {
        if (on_owner()) fail(JP_RF_STATE);
        return;
    }
    struct native_fiber *f=current->running;
    f->wake=current->tick+(uint32_t)ticks; f->indefinite=ticks<0;
    SwitchToFiber(current->scheduler);
}
void rfSleepVFiber(void) { rfSleepFiber(1); }
void rfEndFiber(void) {
    if (!on_owner() || !current || !current->running) {
        if (on_owner()) fail(JP_RF_STATE);
        return;
    }
    current->running->state=ENDED; rfSleepFiber(0);
}
void rfExecFiberServer(struct rf_server *s) {
    if (!access_server(s)) return;
    if (s->executing) { fail(JP_RF_STATE); return; }
    struct rf_server *previous=current;
    s->scheduler=GetCurrentFiber(); s->executing=1; current=s;
    for (int i=0;i<s->capacity;) {
        struct native_fiber *f=s->slots+i; s->running=f;
        if (f->state==ENDED) release(s,f);
        else if (f->state==KILLED) f->state=ENDED;
        else if (f->state==READY && !f->indefinite &&
                 (int32_t)(s->tick-f->wake)>=0) {
            switches++; SwitchToFiber(f->context);
            /* The original SH-4 saved-register context restarts at slot 0
             * after every yield, including sleep(0). Preserve that order. */
            i=0; continue;
        }
        i++;
    }
    s->running=NULL; s->executing=0; s->tick++; passes++; current=previous;
}
