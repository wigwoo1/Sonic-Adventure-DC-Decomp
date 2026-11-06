/* Appended to the qualified sound owner; shares its actual lock and state. */
int jp_sound_exit_claim(uint64_t *out)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(sound.busy||sound_delegation.call)return leave(JP_SOUND_REENTRANT);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    if(!sound.owner||sound.info.phase!=JP_SOUND_READY||sound.info.status||
       sound_delegation.info.token||sound_delegation.info.terminal)return leave(JP_SOUND_STATE);
    if(sound_delegation.info.next_token==UINT64_MAX)return leave(JP_SOUND_STATE);
    uint64_t token=++sound_delegation.info.next_token;
    sound_delegation.info.token=token;sound_delegation.info.closing=0;sound_delegation.info.finished=0;
    *out=token;return leave(0);
}
int jp_sound_exit_cancel(uint64_t token)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(sound.busy||sound_delegation.call)return leave(JP_SOUND_REENTRANT);
    if(!token||token!=sound_delegation.info.token||sound_delegation.info.terminal||
       (sound_delegation.info.closing&&!sound_delegation.info.finished))return leave(JP_SOUND_STATE);
    sound_delegation.info.token=0;sound_delegation.info.closing=sound_delegation.info.finished=0;
    return leave(0);
}
static int sound_delegated_close(uint64_t token,uint32_t timeout,int terminal)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(sound.busy||sound_delegation.call)return leave(JP_SOUND_REENTRANT);
    if(!token||token!=sound_delegation.info.token||(!terminal&&sound_delegation.info.terminal))return leave(JP_SOUND_STATE);
    if(timeout>60000)return leave(JP_SOUND_ARGUMENT);
    if(terminal)sound_delegation.info.terminal=1;
    if(sound_delegation.info.finished)return leave(0);
    sound_delegation.info.closing=1;sound_delegation.call=1;
    ReleaseSRWLockExclusive(&lock);
    rc=jp_sound_shutdown(timeout);
    AcquireSRWLockExclusive(&lock);sound_delegation.call=0;
    if(!rc)sound_delegation.info.finished=1;
    return leave(rc);
}
int jp_sound_exit_close(uint64_t token,uint32_t timeout)
{return sound_delegated_close(token,timeout,0);}
int jp_sound_exit_finish(uint64_t token,uint32_t timeout)
{return sound_delegated_close(token,timeout,1);}
int jp_sound_exit_snapshot(struct jp_sound_exit_info *out)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=sound_delegation.info;return leave(0);
}
