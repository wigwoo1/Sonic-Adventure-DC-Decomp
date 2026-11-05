/* Authored adaptation: order pending AICA output before an ARM RAM store.
 * ARM's callback path synchronizes elapsed CPU cycles before calling here.
 * Instruction fetch and RAM reads retain the pinned core's direct mapping. */
static void EMU_CALL jp_aica_ram_store(void *state,uint32 address,uint32 data,uint32 mask)
{
    yam_flush(YAMSTATE);
    uint32 *word=(uint32 *)(RAMBYTEPTR+(address&(JP_AICA_RAM_MASK&~3U)));
    *word=(*word&~mask)|(data&mask);
}
