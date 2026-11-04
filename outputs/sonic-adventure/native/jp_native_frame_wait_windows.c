#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "jp_native_frame_wait.h"
#include "jp_native_window.h"
int jp_wait_window_idle(void *window,uint32_t milliseconds)
{
    if(!window||milliseconds>50)return -1;
    /* WM_QUIT is a thread condition: preserve it for the application. */
    MSG message;if(PeekMessageW(&message,NULL,WM_QUIT,WM_QUIT,PM_NOREMOVE))return 0;
    if(!jp_window_pump(window))return 0;
    DWORD rc=MsgWaitForMultipleObjectsEx(0,NULL,milliseconds,QS_ALLINPUT,MWMO_INPUTAVAILABLE);
    if(rc==WAIT_FAILED)return -1;
    if(PeekMessageW(&message,NULL,WM_QUIT,WM_QUIT,PM_NOREMOVE))return 0;
    return jp_window_pump(window)?1:0;
}
