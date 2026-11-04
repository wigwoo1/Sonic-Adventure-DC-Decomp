import ctypes as C
from native_packet_test_support import bind
U=C.c_uint32;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;I=C.c_int
class Snapshot(C.Structure):
    _fields_=[('generation',Q),('palette',U*1024),('fog_table',U*128)]+[(n,U) for n in ['palette_mode','fog_color','vertex_fog_color','fog_density','clamp_min','clamp_max','cull_bits','texture_control','fb_write_control','border_color','logical_width','logical_height','clip_x','clip_y','presort','clear_argb','background_isp','background_tsp','background_texture','background_bytes']]+[('background_packet',U*62)]+[(n,U) for n in ['global_clip','global_clip_width','global_clip_height']]
class Context(C.Structure):
    _fields_=[('prefix',U*27),('texture',P),('face',F*4),('offset',F*4),('words',U*5),('bounds',F*4),('tail',U*3)]
class Video:
    def __init__(self,lib):
        self.lib=lib
        size=bind(lib,'jp_video_snapshot_size',S)();assert size==C.sizeof(Snapshot),(size,C.sizeof(Snapshot))
        self.init=bind(lib,'jp_video_sdk_initialize',I,U,U,U,I)
        self.capture=bind(lib,'jp_video_capture',I,C.POINTER(Snapshot));self.frame=bind(lib,'jp_frame_video',I,Q,C.POINTER(Snapshot))
        self.read=bind(lib,'jp_video_read',I,U,C.POINTER(U));self.write=bind(lib,'jp_video_write',I,U,U);self.clear=bind(lib,'jp_video_clear_error',None)
        self.shutdown=bind(lib,'jp_video_shutdown',None);self.palette=bind(lib,'jp_video_palette_rgba',I,C.POINTER(Snapshot),C.POINTER(U))
    def snapshot(self):
        s=Snapshot();assert self.capture(C.byref(s))==0;return s
    def frame_snapshot(self,fid):
        s=Snapshot();assert self.frame(fid,C.byref(s))==0;return s
