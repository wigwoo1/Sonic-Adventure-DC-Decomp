"""ctypes GPU API and independent screen-space reference utilities."""
import ctypes as C
from native_packet_test_support import bind
U=C.c_uint32;S=C.c_size_t;F=C.c_float;P=C.c_void_p;Q=C.c_uint64;I=C.c_int
class Config(C.Structure):
    _fields_=[('width',U),('height',U),('logical_width',F),('logical_height',F),('textures',U),('flat_color_vertex',U),('clear_rgba',F*4),('punch_alpha',F),('cull_small_area',F),('palette',P),('palette_entries',S),('stride_pixels',U),('budget',S)]
    @classmethod
    def make(cls,w=64,h=64,textures=1):
        c=cls();c.width=w;c.height=h;c.logical_width=64;c.logical_height=64;c.textures=textures;c.punch_alpha=0.5;c.budget=64<<20;return c
class Report(C.Structure):
    _fields_=[(n,S) for n in ['batches','triangles','draw_calls','texture_uploads','decoded_texels','accounted_payload_bytes']]+[(n,U) for n in ['width','height','feature_level','textures_enabled','shader_texture_bindings','shader_sampler_bindings']]
class Failure(C.Structure):_fields_=[('code',I),('batch',U),('hresult',C.c_int32)]
class GPU:
    def __init__(self,path):
        self.lib=C.CDLL(str(path));self.open=bind(self.lib,'jp_gpu_open',I,C.POINTER(P),C.POINTER(Failure));self.close=bind(self.lib,'jp_gpu_close',None,P)
        self.draw=bind(self.lib,'jp_gpu_draw_frame',I,P,Q,C.POINTER(Config),P,S,C.POINTER(Report),C.POINTER(Failure));self.handle=P();self.failure=Failure()
        rc=self.open(C.byref(self.handle),C.byref(self.failure));assert rc==0,(rc,self.failure.hresult)
    def image(self,frame,config):
        out=C.create_string_buffer(config.width*config.height*4);report=Report();failure=Failure()
        rc=self.draw(self.handle,frame,C.byref(config),out,len(out),C.byref(report),C.byref(failure))
        return rc,out.raw,report,failure
