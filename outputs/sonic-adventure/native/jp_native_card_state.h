#ifndef JP_NATIVE_CARD_STATE_H
#define JP_NATIVE_CARD_STATE_H
#include <stdint.h>
/* Read-only observation of the original card module; no second card state. */
struct jp_card_state {
    uint32_t mounted,info_read;
    int32_t error,count,maximum;
    uint32_t operation;
    uint8_t disk_info[56];
    int32_t saving,notice,notice_on,selected_drive;
    uint32_t backend_initialized;
    uint64_t original_work_address;
};
__declspec(dllexport) int jp_card_state_read(uint32_t drive,struct jp_card_state *out);
__declspec(dllexport) uint32_t jp_card_work_size(void);
__declspec(dllexport) int jp_card_selected_work_matches(void);
#endif
