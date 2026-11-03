"""Bindings for the clock core's platform provider and independent counters."""
import ctypes as C
U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p
Reader=C.CFUNCTYPE(C.c_int,P,C.POINTER(Q))
class Clock:
    def __init__(self,path):
        self.lib=C.CDLL(str(path))
        def bind(alias,name,result,*args):
            fn=getattr(self.lib,name);fn.restype=result;fn.argtypes=list(args);setattr(self,alias,fn)
        bind('init','jp_clock_initialize',C.c_int,P,P,Q)
        bind('shutdown','jp_clock_shutdown',None)
        bind('status','jp_clock_status',C.c_int)
        bind('wall','jp_clock_wall_counts',Q)
        bind('timer','jp_clock_timer1_ticks',Q)
        bind('tstr','jp_clock_read_tstr',C.c_uint8)
        bind('write','jp_clock_write_tstr',None,C.c_uint8)
        bind('tcnt','jp_clock_read_tcnt1',U)
        bind('reset','jp_clock_reset_timer1',C.c_int,U)
        bind('frequency','jp_clock_frequency',Q)
        bind('sdk_wall','syTmrGetCount',U)
class Counter:
    def __init__(self):
        self.tick=0;self.ok=True
        def read(context,out):
            if not self.ok:return 0
            out[0]=self.tick;return 1
        self.callback=Reader(read)
    def start(self,clock,frequency):return clock.init(C.cast(self.callback,P),None,frequency)
