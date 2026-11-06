/* Observes the real single table used by initialization, QSound and effects.
 * Fixture callers may inspect bytes; no manufactured MIDI/PCM/driver ACKs. */
__declspec(dllexport) int peer_effects_slot(uint32_t slot,int64_t *out)
{
    if(slot>=QUEUE_SLOTS||!out)return 1;const struct sound_slot *s=&sound_slots[slot];
    out[0]=s->priority;out[1]=s->time;out[2]=s->id;out[3]=s->flags;out[4]=s->se;
    out[5]=s->pan;out[6]=s->volume;out[7]=s->loudest;out[8]=s->pitch;out[9]=s->qsound;
    memcpy(out+10,&s->pos,sizeof s->pos);out[12]=sound_volumes[slot];return 0;
}
__declspec(dllexport) int peer_effects_state_addresses(uintptr_t *out)
{
    if(!out)return 1;out[0]=(uintptr_t)&battle_rival;out[1]=(uintptr_t)player_work;
    out[2]=(uintptr_t)&debug_page;out[3]=(uintptr_t)&stage_current;out[4]=(uintptr_t)&act_current;
    out[5]=(uintptr_t)sound_slots;out[6]=(uintptr_t)sound_ports;return 0;
}
void jp_effects_state_owned(struct jp_effects_info *out)
{
    out->muted=sound_muted!=0;out->live_slots=0;
    for(int i=0;i<QUEUE_SLOTS;i++)if(sound_slots[i].time)out->live_slots++;
}
void jp_effects_queue_owned(void){sound_send_queue();}
int jp_effects_request_owned(const struct jp_effects_request *r,int32_t *out)
{
    if((r->kind==1||r->kind==4||r->kind==5)&&!camera_work)return JP_SOUND_DEPENDENCY;
    switch(r->kind){
    case 0:*out=sound_play(r->se,(intptr_t)r->id,r->priority,r->volume);break;
    case 1:*out=sound_play_at(r->se,(intptr_t)r->id,r->priority,r->volume,r->x,r->y,r->z);break;
    case 2:*out=sound_play_held(r->se,(intptr_t)r->id,r->priority,r->volume);break;
    case 3:*out=sound_play_for(r->se,(intptr_t)r->id,r->priority,r->volume,r->updates);break;
    case 4:*out=sound_play_at_for(r->se,(intptr_t)r->id,r->priority,r->volume,r->x,r->y,r->z,r->updates);break;
    default:*out=sound_play_around(r->se,(intptr_t)r->id,r->priority,r->volume,r->x,r->y,r->z,r->radius,r->updates);break;
    }return 0;
}
int jp_effects_load_owned(const char *name,uint32_t kind,int32_t bank)
{
    switch(kind){case 0:sound_load_file(name);break;case 1:sound_load_shot_bank(bank,name);break;
    case 2:sound_load_sequence_bank(bank,name);break;default:sound_load_program_bank(bank,name);break;}
    return jp_effects_failed();
}
int jp_effects_control_owned(uint32_t action,intptr_t id,int32_t value)
{
    switch(action){case 0:sound_stop(value);break;case 1:sound_stop_id(id);break;
    case 2:sound_set_volume(id,value);break;case 3:sound_set_pan(id,value);break;
    case 4:sound_set_pitch(id,value);break;case 5:sound_mute();break;case 6:sound_unmute();break;
    case 7:sound_kill_all();break;default:sound_request_fx(value);break;}return jp_effects_failed();
}
