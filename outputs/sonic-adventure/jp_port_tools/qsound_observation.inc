/* Test inputs/readback for the real shared SDK/game state. No synthetic audio,
 * command builder, spatial implementation or driver acknowledgement. */
_Static_assert(sizeof(qsound_owners[0][0])==sizeof(void *),"native QSound identities");
_Static_assert(sizeof(((struct sound_slot *)0)->id)==sizeof(void *),"native slot identities");
_Static_assert(sizeof(struct sd_host_command)==20,"SDK host command keeps32bit wire fields");
int32_t jp_game_qsound_set(const int8_t *positions)
{
    jp_game_qsound_result=SDE_ERR_UNKNOW_NUM;
    jp_game_sound_set_qsound((Sint8 *)positions);return (int32_t)jp_game_qsound_result;
}
__declspec(dllexport) int peer_qsound_prepare(uint32_t slot,int32_t se,intptr_t id)
{
    if(slot>=QUEUE_SLOTS)return 1;
    sound_slots[slot].se=se;sound_slots[slot].id=id;sound_slots[slot].qsound=-2;return 0;
}
__declspec(dllexport) int peer_qsound_allocate(uint32_t slot,int32_t *copy)
{
    if(slot>=QUEUE_SLOTS||!copy)return 1;*copy=qsound_alloc((int)slot);return 0;
}
__declspec(dllexport) int peer_qsound_free(uint32_t slot)
{if(slot>=QUEUE_SLOTS)return 1;qsound_free((int)slot);return 0;}
__declspec(dllexport) int peer_qsound_range(int32_t se){return qsound_range(se);}
__declspec(dllexport) int peer_qsound_owners(uint64_t *out,uint32_t count)
{
    if(!out||count<QSOUND_SES*QSOUND_COPIES)return 1;
    for(int row=0;row<QSOUND_SES;row++){
        for(int copy=0;copy<QSOUND_COPIES;copy++)out[row*QSOUND_COPIES+copy]=qsound_owners[row][copy];
    }
    return 0;
}
__declspec(dllexport) int peer_qsound_slot(uint32_t slot,uint64_t *out)
{
    if(slot>=QUEUE_SLOTS||!out)return 1;
    out[0]=(uintptr_t)sound_slots[slot].id;out[1]=(uint64_t)(int64_t)sound_slots[slot].se;
    out[2]=(uint64_t)(int64_t)sound_slots[slot].qsound;return 0;
}
__declspec(dllexport) int peer_qsound_error(uint64_t *out)
{
    if(!out)return 1;out[0]=(uint32_t)sdgErrResult;out[1]=(uintptr_t)sdgErrApi;return 0;
}
__declspec(dllexport) int peer_qsound_entry(uint32_t index,void *out)
{
    if(!sdgWrkPtr||!out||index>=SD_2ND_HOST_CMD_MAX)return 1;
    memcpy(out,&sdgWrkPtr->host.second[index],sizeof(struct sd_host_command_entry));return 0;
}
__declspec(dllexport) void *peer_qsound_second(void)
{return sdgWrkPtr?sdgWrkPtr->host.second:NULL;}
