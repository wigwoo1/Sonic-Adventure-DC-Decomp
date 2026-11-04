"""Typed job/query APIs and bounded hardware polling; no fake completions."""
import ctypes as C,time
from native_packet_test_support import bind
from native_gpu_test_support import Report,Failure
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t
class Timing(C.Structure):
    _fields_=[(n,Q) for n in ['id','frame','cpu_frequency','cpu_begin','cpu_prepared','cpu_submitted','cpu_completed_observed','gpu_frequency','gpu_begin','gpu_end','gpu_elapsed_counts']]+[(n,U) for n in ['gpu_timing_valid','width','height','reserved']]
class Jobs:
    def __init__(self,gpu):
        self.gpu=gpu;lib=gpu.lib
        assert bind(lib,'jp_gpu_timing_size',S)()==C.sizeof(Timing)
        self.submit=bind(lib,'jp_gpu_submit_frame',I,P,Q,P,P,P,P)
        self.configured=bind(lib,'jp_gpu_submit_configured_frame',I,P,Q,P,P,P,P)
        self.poll=bind(lib,'jp_gpu_job_poll',I,P,Q,P,P);self.release=bind(lib,'jp_gpu_job_release',I,P,Q,P)
        self.live=bind(lib,'jp_gpu_jobs_live',U,P);self.copy=bind(lib,'jp_window_copy_job',I,P,Q,P);self.present=bind(lib,'jp_window_present_job',I,P,Q,U,P)
        self.convert=bind(lib,'jp_gpu_elapsed_counts',I,Q,Q,Q,P)
        kernel=C.WinDLL('kernel32');self.counter=bind(kernel,'QueryPerformanceCounter',I,P);self.frequency=bind(kernel,'QueryPerformanceFrequency',I,P)
        value=C.c_int64();assert self.frequency(C.byref(value)) and value.value>0;self.hz=value.value
    def now(self):
        value=C.c_int64();assert self.counter(C.byref(value)) and value.value>=0;return value.value
    def wait(self,id,timeout=10):
        deadline=time.monotonic()+timeout;pending=0;t=Timing();e=Failure()
        while True:
            C.memset(C.byref(t),0x5a,C.sizeof(t));saved=bytes(t);before=self.now();code=self.poll(self.gpu.handle,id,C.byref(t),C.byref(e));after=self.now()
            if code==0:
                assert t.id==id and t.cpu_frequency==self.hz and t.cpu_begin<=t.cpu_prepared<=t.cpu_submitted<=t.cpu_completed_observed<=after
                assert t.reserved==0 and t.gpu_timing_valid in [0,1]
                if t.gpu_timing_valid:
                    assert t.gpu_frequency>0 and t.gpu_end>=t.gpu_begin
                    assert t.gpu_elapsed_counts==(t.gpu_end-t.gpu_begin)*781250//t.gpu_frequency
                else:assert t.gpu_elapsed_counts==0
                return t,pending
            assert code==100 and e.code==100 and bytes(t)==saved,(code,e.hresult)
            assert time.monotonic()<deadline,'Actual GPU completion exceeded bounded test wait'
            pending+=1;time.sleep(.001)
