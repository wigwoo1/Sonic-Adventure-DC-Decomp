import ctypes as C
from native_packet_test_support import bind
from native_clock_test_support import Clock
U=C.c_uint32;I=C.c_int;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
class Info(C.Structure):
    _fields_=[(n,Q) for n in ['frame','job','submissions','completions','presentations','eor_calls','submitted_tick','required_tick','presented_tick','timeouts']]+[(n,U) for n in ['status','ready','history','gpu_timing_valid','timeout_ms','interval']]
class Stats(C.Structure):
    _fields_=[(n,U) for n in ['render_wait','ta_wait','render_timeouts','ta_timeouts','ol_overflow_wait','isp_overflow_wait','hold_wait','ol_overflows','isp_overflows']]+[('isp_current',U*2),('list_max',U*5)]+[(n,U) for n in ['render_start','render_time','ta_start','ta_time']]
class Present:
    def __init__(self,path):
        self.clock=Clock(path);self.lib=lib=self.clock.lib
        self.init=bind(lib,'jp_present_initialize',I,P,P,P,P,U,U);self.shutdown=bind(lib,'jp_present_shutdown',I);self.status=bind(lib,'jp_present_status',I)
        self.info=bind(lib,'jp_present_snapshot',I,P);self.stats=bind(lib,'jp_present_read_stats',I,P,S);self.poll=bind(lib,'jp_present_poll',I)
        self.valid=bind(lib,'jp_present_timing_ready',I);assert bind(lib,'jp_present_stats_size',S)()==C.sizeof(Stats)
        self.render=bind(lib,'kmRender',U);self.flip=bind(lib,'kmFlipFrameBuffer',U);self.callback=bind(lib,'kmSetEORCallback',U,P,P);self.reset=bind(lib,'kmResetPointer',None)
        self.wait_init=bind(lib,'jp_wait_initialize',I,U,U,U,P,P);self.wait_shutdown=bind(lib,'jp_wait_shutdown',I);self.wait_status=bind(lib,'jp_wait_status',I)
        self.window_idle=bind(lib,'jp_wait_window_idle',I,P,U);self.pacing=bind(lib,'frame_pacing_init',None,I,I);self.frame=bind(lib,'frame_wait',None)
        self.probe=bind(lib,'jp_wait_probe_reset',None,U,U);self.counter=bind(lib,'jp_wait_probe_counter',I,U);self.clear=bind(lib,'jp_wait_probe_clear_events',None)
        self.busy=bind(lib,'jp_present_probe_busy',U,U);self.device=bind(lib,'jp_present_probe_device',None,P);self.fits=bind(lib,'jp_present_probe_fits',I,U)
    def snapshot(self):
        result=Info();assert self.info(C.byref(result))==0;return result
    def statistics(self):
        result=Stats();assert self.stats(C.byref(result),C.sizeof(result))==0;return result
