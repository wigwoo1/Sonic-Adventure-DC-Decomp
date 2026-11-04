"""ctypes declarations for the authored platform-neutral packet plan."""
import ctypes as C
U=C.c_uint32;S=C.c_size_t;F=C.c_float;P=C.c_void_p
class Span(C.Structure):_fields_=[('data',P),('bytes',S)]
class Failure(C.Structure):_fields_=[('code',C.c_int),('list',U),('offset',S)]
class Vertex(C.Structure):_fields_=[(n,F) for n in ['x','y','inv_w','u','v']]+[('base',F*4),('offset',F*4),('base_intensity',F),('offset_intensity',F)]
class Triangle(C.Structure):_fields_=[('indices',S*3)]
class Batch(C.Structure):_fields_=[('header',U*16)]+[(n,U) for n in ['header_words','list','vertex_type']]+[(n,S) for n in ['source_offset','first_vertex','vertex_count','first_triangle','triangle_count']]
class Plan(C.Structure):_fields_=[(n,S) for n in ['allocation_bytes','batch_count','vertex_count','triangle_count']]+[('batches',C.POINTER(Batch)),('vertices',C.POINTER(Vertex)),('triangles',C.POINTER(Triangle))]
def bind(lib,name,result,*args):
    f=getattr(lib,name);f.restype=result;f.argtypes=list(args);return f
class Decoder:
    def __init__(self,dll):
        self.dll=C.CDLL(str(dll))
        self.decode=bind(self.dll,'jp_packets_decode',C.c_int,C.POINTER(Span),S,C.POINTER(P),C.POINTER(Failure))
        self.frame=bind(self.dll,'jp_packets_decode_frame',C.c_int,C.c_uint64,S,C.POINTER(P),C.POINTER(Failure))
        self.destroy=bind(self.dll,'jp_packets_destroy',None,P)
    def raw(self,lists,budget=64<<20,sentinel=0x12345678):
        blobs=[C.create_string_buffer(b) if b else None for b in lists]
        spans=(Span*5)(*(Span(C.addressof(b) if b else None,len(raw)) for b,raw in zip(blobs,lists)))
        out=P(sentinel);failure=Failure();rc=self.decode(spans,budget,C.byref(out),C.byref(failure))
        return rc,out,failure
