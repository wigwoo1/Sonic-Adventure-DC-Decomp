/* Explicit fixture input/readback for the original shared sound-slot table.
 * These calls do not manufacture a camera, queue producer or player state.
 */
__declspec(dllexport) int peer_qsound_position(uint32_t slot,const void *position)
{
    if(slot>=QUEUE_SLOTS||!position)return 1;
    memcpy(&sound_slots[slot].pos,position,sizeof(NJS_VECTOR));return 0;
}
__declspec(dllexport) int peer_qsound_position_readback(uint32_t slot,void *position)
{
    if(slot>=QUEUE_SLOTS||!position)return 1;
    memcpy(position,&sound_slots[slot].pos,sizeof(NJS_VECTOR));return 0;
}
__declspec(dllexport) int peer_qsound_place(uint32_t slot,int32_t *out)
{
    if(slot>=QUEUE_SLOTS||!out)return 1;
    *out=qsound_place((int)slot);return 0;
}
