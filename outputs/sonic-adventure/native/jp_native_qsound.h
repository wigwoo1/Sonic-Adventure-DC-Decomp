#ifndef JP_NATIVE_QSOUND_H
#define JP_NATIVE_QSOUND_H
#include <stdint.h>
#ifndef JP_QSOUND_API
#define JP_QSOUND_API __declspec(dllexport)
#endif
/* The original game wrapper and SDK producer read exactly seven signed-byte
 * positions, copying their bits into a real queued command. bytes>=7; no
 * pointer is retained. Requires the actual ready creator-thread sound owner.
 * Original byte/changed-bit semantics are preserved, without host clamping.
 * Returns existing jp_sound_error values. A full SDK host queue returns
 * JP_SOUND_QUEUE without quarantining the otherwise healthy sound owner.
 * Full spatial placement/game queue and acoustic 3D fidelity remain separate.
 */
JP_QSOUND_API int jp_sound_set_qsound(const int8_t *positions,uint32_t bytes);
#endif
