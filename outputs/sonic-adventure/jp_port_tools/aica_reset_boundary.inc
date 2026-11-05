/* Authored internal bridge appended to the privately generated external core. */
extern void jp_aica_arm_reset(void *);
void jp_aica_core_cpu_reset(void *state)
{
    yam_flush(YAMSTATE);
    jp_aica_arm_reset(ARMSTATE);
    arm_set_advance_callback(ARMSTATE,dcsound_advance,DCSOUNDSTATE);
    arm_set_memory_maps(ARMSTATE,MAPLOAD,MAPSTORE);
    DCSOUNDSTATE->cycles_ahead_of_sound=0;
    DCSOUNDSTATE->cycles_executed=0;
}
void jp_aica_core_held_render(void *state,sint16 *out,uint32 frames)
{
    yam_beginbuffer(YAMSTATE,out);
    yam_advance(YAMSTATE,frames);
    yam_flush(YAMSTATE);
}
void jp_aica_core_flush(void *state){yam_flush(YAMSTATE);}
