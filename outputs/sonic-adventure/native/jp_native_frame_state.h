/* Shared original frame counters. Exactly one native DLL owns their storage.
 * SDK/game functions keep using their original names and scalar types. */
#ifndef JP_NATIVE_FRAME_STATE_H
#define JP_NATIVE_FRAME_STATE_H
#if defined(JP_FRAME_STATE_OWNER)
#define JP_FRAME_STATE_API __declspec(dllexport)
#else
#define JP_FRAME_STATE_API __declspec(dllimport)
#endif
JP_FRAME_STATE_API extern int frame_interval;
JP_FRAME_STATE_API extern int frame_countdown;
JP_FRAME_STATE_API extern int frame_step_count;
JP_FRAME_STATE_API extern int frame_step;
#endif
