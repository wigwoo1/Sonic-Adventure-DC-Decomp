/* Compiled after the privately extracted original card lifecycle. These are
 * observation functions, not stand-ins for any game or SDK callback. */
_Static_assert(sizeof(BUS_DISKINFO)==56,"original disk-info host scalar layout");
extern Uint32 buInitializeFlag;
int jp_card_state_read(uint32_t drive,struct jp_card_state *out) {
    if (drive>=CARD_DRIVES || !out) return 1;
    const struct card_drive *record=&card_drives[drive];
    memset(out,0,sizeof(*out));
    out->mounted=record->mounted; out->info_read=record->info_read;
    out->error=record->error; out->count=record->count;
    out->maximum=record->max; out->operation=record->op;
    memcpy(out->disk_info,&record->info,sizeof(record->info));
    out->saving=card_saving; out->notice=card_notice;
    out->notice_on=card_notice_on; out->selected_drive=save_drive;
    out->backend_initialized=buInitializeFlag;
    out->original_work_address=(uintptr_t)card_library_work;
    return 0;
}
uint32_t jp_card_work_size(void) { return sizeof(card_library_work); }
int jp_card_selected_work_matches(void) {
    return save_drive>=0 && save_drive<CARD_DRIVES &&
        buVmsDrive[save_drive].work==(Uint8 *)card_library_work;
}
