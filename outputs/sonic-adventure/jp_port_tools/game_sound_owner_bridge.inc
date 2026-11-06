/* Same translation unit as the genuine SDK and original game initialization.
 * These bridges observe/retire actual state; they do not fabricate commands,
 * ready flags, MIDI ports, driver acknowledgements or PCM. */
int32_t jp_game_sdk_service(void){return (int32_t)sdSysServer();}
int jp_game_sdk_active(void){return sdgWrkPtr!=NULL||sdgExistSndDrv!=false;}
void jp_game_sdk_retire(void)
{
    /* The owner has unbound frame callbacks and closed G2 before this call. */
    sdgExistSndDrv=false;sdgWrkPtr=NULL;sdgG2DmaHandle=NULL;
    sdgMemBlkDbPtr=NULL;sdgMemBlkQueDbPtr=NULL;sdgMemBlkQueBufPtr=NULL;
    sdgMemBlkQueBufNum=0;sdgPrimaryBufPtr=NULL;sdg2ndHostCmdBufPtr=NULL;
    sdgG2LibResult=SYE_G2_ERR_NOTHING;sdgErrApi=NULL;sdgErrResult=SDE_ERR_NOTHING;
    sdgPrintFunc=NULL;sdgDebugMode=false;jp_sd_boot_frames=0;
    sdgMemBlkG2DmaErrNum=sdgG2DmaTriggerNum=sdgG2DmaEndNum=0;
    memset(sdgDefaultWrkImg,0,sizeof sdgDefaultWrkImg);
    memset(sdgDefMemBlkBuf,0,sizeof sdgDefMemBlkBuf);
    memset(sdgDefMemBlkQue,0,sizeof sdgDefMemBlkQue);
    memset(sdgDefPrimaryHostCmdBuf,0,sizeof sdgDefPrimaryHostCmdBuf);
    memset(sdgDef2ndHostCmdBuf,0,sizeof sdgDef2ndHostCmdBuf);
    memset(sound_ports,0,sizeof sound_ports);memset(sound_slots,0,sizeof sound_slots);
    memset(&midi_message,0,sizeof midi_message);memset(midi_keys,0,sizeof midi_keys);
    memset(sound_volumes,0,sizeof sound_volumes);memset(qsound_owners,0,sizeof qsound_owners);
    memset(qsound_pos,0,sizeof qsound_pos);memset(qsound_sent,0,sizeof qsound_sent);
    sound_muted=midi_side=sound_next_channel=sound_next_sequence=0;
    sound_fx_request=sound_fx_none=0;
}
/* Passive observation of actual game BSS and pointers for independent tests. */
__declspec(dllexport) int peer_game_sound(uint64_t *out)
{
    if(!out)return 1;
    out[0]=sound_muted;out[1]=sound_next_channel;out[2]=sound_next_sequence;
    out[3]=(uint64_t)(int64_t)sound_fx_request;out[4]=(uint64_t)(int64_t)sound_fx_none;
    out[5]=midi_side;
    for(int i=0;i<PORT_COUNT;i++)out[6+i]=(uintptr_t)sound_ports[i];
    uint32_t slots=0,owners=0,positions=0,sent=0;
    for(int i=0;i<QUEUE_SLOTS;i++)if(sound_slots[i].time)slots++;
    for(int i=0;i<QSOUND_SES;i++)for(int j=0;j<QSOUND_COPIES;j++)if(qsound_owners[i][j])owners++;
    for(int i=0;i<QSOUND_CHANNELS;i++){if(qsound_pos[i]==QSOUND_RESET_PLACE)positions++;if(qsound_sent[i])sent++;}
    out[14]=slots;out[15]=owners;out[16]=positions;out[17]=sent;return 0;
}
