"""Native SDK texture layouts and exports for real-runtime checks."""
import ctypes as C
U=C.c_uint32;P=C.c_void_p
class Surface(C.Structure):
    _fields_=[(n,U) for n in ['Type','BitDepth','PixelFormat','nWidth','nHeight','TextureSize','fSurfaceFlags']]+[(n,P) for n in ['pSurface','pVirtual','pPhysical']]
class Info(C.Structure):_fields_=[('texaddr',P),('texsurface',Surface)]
class Record(C.Structure):
    _fields_=[(n,U) for n in ['globalIndex','bank','tspparambuffer','texparambuffer','texaddr']]+[('texinfo',Info),('count',C.c_uint16),('dummy',C.c_uint16)]
class Name(C.Structure):_fields_=[('filename',P),('attr',U),('texaddr',U)]
class List(C.Structure):_fields_=[('textures',C.POINTER(Name)),('nbTexture',U)]
def field(name):return C.cast(C.byref(name,Name.texaddr.offset),C.POINTER(U))
class Runtime:
    def __init__(self,path):
        self.dll=C.CDLL(str(path))
        def b(alias,name,result,*args):
            f=getattr(self.dll,name);f.restype=result;f.argtypes=list(args);setattr(self,alias,f)
        b('init','jp_textures_initialize',C.c_int,U,U,C.c_size_t)
        b('shutdown','jp_textures_shutdown',None)
        b('status','jp_textures_status',C.c_int)
        b('clear','jp_textures_clear_error',None)
        b('allocated','jp_textures_allocated_bytes',C.c_size_t)
        b('live','jp_textures_live_surfaces',U)
        b('record','jp_textures_record',P,U)
        b('resolve','jp_texture_resolve',P,U)
        b('resolve_field','jp_texture_resolve_field',P,C.POINTER(U))
        b('lookup','jp_renderer_texture_lookup',P,C.POINTER(List),U)
        b('store','jp_texture_store_handle',C.c_int,C.POINTER(U),P)
        b('forget','jp_texture_forget_field',None,C.POINTER(U))
        b('copy','jp_texture_copy_surface',C.c_int,C.POINTER(Info),P,C.c_size_t)
        b('surface_word','jp_texture_surface_word',U,C.POINTER(Surface))
        b('surface_data','jp_texture_surface_data',P,U)
        b('surface_bytes','jp_texture_surface_bytes',C.c_size_t,U)
        b('setname','njSetTextureName',None,C.POINTER(Name),P,U,U)
        b('entry','njEntryTexMemList',None,C.POINTER(U),C.POINTER(Info),U,U,U)
        b('free','njFreeTexMemList',None,P)
        b('nextinfo','njTexMemList',P,U)
        b('param','njSetTextureParam',None,P)
        b('palette','njSetPaletteBankLN',None,P,C.c_int32)
        b('select','njSetTexture',C.c_int32,C.POINTER(List))
        b('release','njReleaseTextureLN',C.c_int32,U)
        b('release_num','njReleaseTextureNum',C.c_int32,U)
        b('release_list','njReleaseTexture',C.c_int32,C.POINTER(List))
        b('search','njSearchTexList',C.c_int32,P,U,U)
        b('search2','njSearchTexList2',C.c_int32,P,U,P)
        b('search3','njSearchTexList3',C.c_int32,P,U,U,U)
        b('set_flags','njSetFlagAddrAllTexture',None,U,U,P)
        self.count=C.c_int32.in_dll(self.dll,'_nj_tex_count')
        self.current=U.in_dll(self.dll,'_nj_current_texture')
    def at(self,index):return Record.from_address(self.record(index))
