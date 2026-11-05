#ifndef JP_NATIVE_DEVICE_INTERNAL_H
#define JP_NATIVE_DEVICE_INTERNAL_H
#include "jp_native_device.h"
int jp_device_fail(int);
int jp_device_wait_tick(void);
int jp_device_create_gpu(void);
int jp_device_create_display(unsigned,unsigned,void *);
int jp_device_phase_is(unsigned);
int jp_device_set_cable(unsigned);
int jp_device_sdk_validate(int32_t,int32_t,int32_t);
int jp_device_sdk_video(unsigned);
void jp_device_sdk_snapshot(struct jp_device_info *);
void jp_original_ninja_system(int32_t,int32_t,int32_t);
#endif
