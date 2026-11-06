/* Appended to the same actual motion owner; no state, allocation or mutation. */
__declspec(dllexport) int jp_motions_can_select(int32_t character)
{
    int rc=access_owner();if(rc)return rc;
    static const int8_t bank_for_character[8]={0,-1,0,4,-1,1,3,2};
    if(character<0||character>=8||bank_for_character[character]<0)return JP_MOTION_CHARACTER;
    uint32_t bank=(uint32_t)bank_for_character[character];
    if(bank_busy(bank)||(store.info.selected_bank<5&&bank_busy(store.info.selected_bank)))return JP_MOTION_BUSY;
    return next_generation?0:JP_MOTION_LIMIT;
}
