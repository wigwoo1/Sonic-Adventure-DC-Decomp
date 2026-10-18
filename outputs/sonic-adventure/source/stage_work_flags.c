/* Original 0x8c07c65e..0x8c07c675. Native work table replaces retail
 * pointer table at 0x8c78c608. Field meanings beyond offsets remain unknown. */
#include "stage00_task.h"
struct sa_flagged_work { uint8_t field0, field1, reserved[2]; uint16_t flags; };
SA_STAGE_EXPORT void sa_stage_work_flags_8c07c65e(
 struct sa_flagged_work *const table[256],uint32_t index,uint32_t value) {
 struct sa_flagged_work *work=table[index & 255u];
 work->flags |= 0x1000u;
 work->field1=(uint8_t)value;
}
