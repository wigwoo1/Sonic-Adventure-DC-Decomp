/* Shares the actual sound owner's creator, lock, SDK and AICA. */
_Static_assert(sizeof(struct jp_sound_clock_options)==56,"sound clock options ABI");
_Static_assert(sizeof(struct jp_sound_clock_info)==136,"sound clock snapshot ABI");
static int sound_clock_error(int rc,uint32_t native)
{sound_clock.info.phase=JP_SOUND_CLOCK_FAILED;sound_clock.info.status=(uint32_t)rc;sound_clock.info.native_error=native;return rc;}
static int sound_clock_native_wall(void *ctx,uint64_t *out)
{(void)ctx;*out=jp_clock_wall_counts();return jp_clock_status()==0;}
static int32_t sound_clock_wait_tick(void *ctx)
{(void)ctx;return jp_sound_clock_tick();}
static int sound_clock_account(void)
{
    uint64_t now=0;
    if(!sound_clock.options.source(sound_clock.options.context,&now))return sound_clock_error(JP_SOUND_CLOCK_SOURCE,0);
    if(now<sound_clock.info.last_tick)return sound_clock_error(JP_SOUND_CLOCK_BACKWARDS,0);
    uint64_t hz=sound_clock.info.frequency,delta=now-sound_clock.info.last_tick;
    uint64_t seconds=delta/hz,part=(delta%hz)*44100+sound_clock.info.fraction;
    if(seconds>UINT64_MAX/44100)return sound_clock_error(JP_SOUND_CLOCK_OVERFLOW,0);
    uint64_t whole=seconds*44100,extra=part/hz;
    if(whole>UINT64_MAX-extra||sound_clock.info.due_frames>UINT64_MAX-(whole+extra))return sound_clock_error(JP_SOUND_CLOCK_OVERFLOW,0);
    sound_clock.info.last_tick=now;sound_clock.info.fraction=part%hz;
    sound_clock.info.due_frames+=whole+extra;sound_clock.info.backlog=sound_clock.info.due_frames-sound_clock.info.rendered_frames;
    return 0;
}
static int sound_clock_cancel(uint32_t cancel)
{
    if(sound_clock.info.in_tick)return JP_SOUND_CLOCK_REENTRANT;
    if(!cancel&&sound_clock.info.backlog)return JP_SOUND_CLOCK_PENDING;
    if(sound_clock.info.wait_token){
        int rc=jp_wait_service_unbind(sound_clock.info.wait_token);
        if(rc)return JP_SOUND_CLOCK_WAIT;
        sound_clock.info.wait_token=0;
    }
    if(cancel){sound_clock.info.cancelled_frames+=sound_clock.info.backlog;sound_clock.info.backlog=0;}
    sound_clock.options.source=NULL;sound_clock.options.context=NULL;
    sound_clock.options.activity=NULL;sound_clock.options.activity_context=NULL;
    if(sound_clock.info.phase!=JP_SOUND_CLOCK_COLD)sound_clock.info.phase=JP_SOUND_CLOCK_STOPPED;
    return 0;
}
/* Original cleanup already holds sound.busy, and never holds SRW across calls. */
static int jp_sound_clock_close_owned(void)
{
    if(sound_clock.info.wait_token&&sound_clock.info.phase==JP_SOUND_CLOCK_ACTIVE){
        sound_clock.info.in_tick=1;(void)sound_clock_account();sound_clock.info.in_tick=0;}
    return sound_clock_cancel(1);
}
int jp_sound_clock_start(const struct jp_sound_clock_options *options)
{
    int rc=begin(1);if(rc)return rc;
    if(sound_clock.info.wait_token)return end(JP_SOUND_CLOCK_STATE);
    if(!options||options->bytes!=sizeof *options||!options->quantum_frames||options->quantum_frames>2048||
       options->max_frames_per_tick<options->quantum_frames||options->max_frames_per_tick>8192||
       !options->period_ms||options->period_ms>10||(!options->source&&(options->context||options->frequency))||
       (options->source&&(!options->frequency||options->frequency>UINT64_MAX/44101))||
       (!options->activity&&options->activity_context))return end(JP_SOUND_CLOCK_ARGUMENT);
    if(!(sound.options.aica.flags&JP_AICA_WITH_OUTPUT))return end(JP_SOUND_CLOCK_STATE);
    sound_clock.options=*options;
    if(!options->source){sound_clock.options.source=sound_clock_native_wall;sound_clock.options.frequency=781250;}
    struct jp_aica_info core;
    if(jp_aica_snapshot(&core)||core.phase!=JP_AICA_READY||!core.output_enabled)return end(JP_SOUND_CLOCK_STATE);
    struct jp_sound_clock_info fresh={.phase=JP_SOUND_CLOCK_ACTIVE,.creator_thread=GetCurrentThreadId(),
        .frequency=sound_clock.options.frequency,.initial_core_frames=core.frames,
        .quantum_frames=options->quantum_frames,.max_frames_per_tick=options->max_frames_per_tick,.period_ms=options->period_ms};
    sound_clock.info=fresh;sound_clock.info.in_tick=1;uint64_t now=0;
    int sampled=sound_clock.options.source(sound_clock.options.context,&now);sound_clock.info.in_tick=0;
    if(!sampled){sound_clock_error(JP_SOUND_CLOCK_SOURCE,0);return end(JP_SOUND_CLOCK_SOURCE);}
    sound_clock.info.anchor=sound_clock.info.last_tick=now;
    rc=jp_wait_service_bind(sound_clock_wait_tick,&sound_clock,options->period_ms,&sound_clock.info.wait_token);
    if(rc){sound_clock_error(JP_SOUND_CLOCK_WAIT,(uint32_t)rc);return end(JP_SOUND_CLOCK_WAIT);}
    return end(0);
}
int jp_sound_clock_tick(void)
{
    int rc=begin(1);if(rc)return rc;
    if(sound_clock.info.phase!=JP_SOUND_CLOCK_ACTIVE||!sound_clock.info.wait_token)return end(JP_SOUND_CLOCK_STATE);
    sound_clock.info.in_tick=1;
    if(sound_clock.options.activity){rc=sound_clock.options.activity(sound_clock.options.activity_context,1);
        if(rc){sound_clock.info.in_tick=0;return end(rc);}}
    rc=sound_clock_account();if(rc)goto done;
    uint32_t budget=sound_clock.options.max_frames_per_tick;
    while(sound_clock.info.backlog&&budget){
        if(sound_clock.info.service_calls==UINT64_MAX){rc=sound_clock_error(JP_SOUND_CLOCK_OVERFLOW,0);goto done;}
        int32_t sdk=jp_game_sdk_service();sound_clock.info.service_calls++;
        if(sdk){sound.info.sdk_error=sdk;fail(JP_SOUND_SDK,JP_SOUND_OWNS_SDK,(uint32_t)sdk);rc=sound_clock_error(JP_SOUND_CLOCK_SDK,(uint32_t)sdk);goto done;}
        uint32_t n=sound_clock.options.quantum_frames;
        if(n>budget)n=budget;if(n>sound_clock.info.backlog)n=(uint32_t)sound_clock.info.backlog;
        int native=jp_aica_pump(n);
        if(native==JP_AICA_QUEUE){if(sound_clock.info.queue_full==UINT64_MAX){rc=sound_clock_error(JP_SOUND_CLOCK_OVERFLOW,0);goto done;}sound_clock.info.queue_full++;break;}
        if(native){fail(JP_SOUND_AICA,JP_SOUND_OWNS_AICA,(uint32_t)native);rc=sound_clock_error(JP_SOUND_CLOCK_OUTPUT,(uint32_t)native);goto done;}
        sound_clock.info.rendered_frames+=n;sound_clock.info.backlog-=n;sound_clock.info.render_calls++;budget-=n;
    }
done:
    if(sound_clock.options.activity){int guard=sound_clock.options.activity(sound_clock.options.activity_context,0);
        if(guard&&!rc)rc=sound_clock_error(JP_SOUND_CLOCK_WAIT,(uint32_t)guard);}
    sound_clock.info.in_tick=0;return end(rc);
}
int jp_sound_clock_stop(uint32_t cancel)
{
    int rc=begin(0);if(rc)return rc;
    if(cancel>1)return end(JP_SOUND_CLOCK_ARGUMENT);
    jp_sound_clock_activity activity=sound_clock.options.activity;void *ctx=sound_clock.options.activity_context;
    if(activity){rc=activity(ctx,1);if(rc)return end(rc);}
    if(sound_clock.info.wait_token&&sound_clock.info.phase==JP_SOUND_CLOCK_ACTIVE){
        sound_clock.info.in_tick=1;rc=sound_clock_account();sound_clock.info.in_tick=0;
        if(rc&&!cancel){if(activity)(void)activity(ctx,0);return end(rc);}}
    rc=sound_clock_cancel(cancel);if(activity){int guard=activity(ctx,0);if(!rc)rc=guard;}return end(rc);
}
int jp_sound_clock_snapshot(struct jp_sound_clock_info *out)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_CLOCK_ARGUMENT);
    *out=sound_clock.info;return leave(0);
}
