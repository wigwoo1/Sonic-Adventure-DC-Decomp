"""Native PVM APIs, synthetic archives and independent encoded pixel oracle."""
import ctypes as C,struct
from native_packet_test_support import bind
from native_texture_test_support import Name,List
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t
class PVM:
    def __init__(self,lib):
        self.load=bind(lib,'jp_pvm_load_memory',I,P,U,S,P,S)
        self.sdk=bind(lib,'njLoadTexturePvmMemory',I,P,P)
        self.destination=bind(lib,'jp_pvm_bind_destination',I,P,U,S)
        self.unbind=bind(lib,'jp_pvm_unbind_destination',I)
        self.active=bind(lib,'jp_pvm_destination_active',I)
        self.loaded=bind(lib,'jp_pvm_loaded_count',U,P)
        self.release=bind(lib,'jp_pvm_release_loaded',I,P)
        self.begin=bind(lib,'jp_memory_bind_source',I,P,S);self.end=bind(lib,'jp_memory_unbind_source',I)
        self.status=bind(lib,'jp_memory_status',I);self.clear=bind(lib,'jp_memory_clear_error',None);self.shutdown=bind(lib,'jp_memory_shutdown',I)
        self.stride=bind(lib,'njCalcPvmStatusSize',I,C.c_uint16)
        self.context=P.in_dll(lib,'_nj_current_ctx_')
    def bank(self):return (U*27).from_address(self.context.value)[26]
def chunk(tag,data):return tag+struct.pack('<I',len(data))+data
def texture(data,kind=0x901,w=8,h=8):return chunk(b'PVRT',struct.pack('<IHH',kind,w,h)+data)
def palette(mode,bank,offset,colors):
    width=4 if mode==6 else 2
    return chunk(b'PVPL',struct.pack('<Hhhh',mode,bank,offset,len(colors))+b''.join(int(c).to_bytes(width,'little') for c in colors))
def archive(textures,flags=9,numbers=None,gids=None,palettes=b'',names=None):
    n=len(textures);numbers=list(range(n)) if numbers is None else numbers;gids=[1000+i for i in range(n)] if gids is None else gids
    names=[('texture_'+str(i)).encode().ljust(28,b'\0') for i in range(n)] if names is None else names
    entries=[]
    for i in range(n):
        e=struct.pack('<H',numbers[i])
        if flags&1:e+=names[i]
        if flags&2:e+=b'CT'
        if flags&4:e+=b'EN'
        if flags&8:e+=struct.pack('<I',gids[i])
        entries.append(e)
    return chunk(b'PVMH',struct.pack('<Hh',flags,n)+b''.join(entries))+palettes+b''.join(textures)
def destination(n,gids=None):
    names=(Name*n)();buffers=[]
    for i in range(n):
        buf=C.create_string_buffer(b'\xa5'*64);buffers.append(buf);names[i].filename=C.addressof(buf)+16;names[i].texaddr=(gids[i] if gids is not None else 9000+i)
    return names,List(names,n),buffers
def index_archive(data):
    assert data[:4]==b'PVMH';size,flags,count=struct.unpack_from('<IHh',data,4)
    stride=2+(28 if flags&1 else 0)+(2 if flags&2 else 0)+(2 if flags&4 else 0)+(4 if flags&8 else 0)
    rows=[]
    for i in range(count):
        off=12+i*stride;number=struct.unpack_from('<H',data,off)[0];gid=struct.unpack_from('<I',data,off+stride-4)[0] if flags&8 else None
        rows.append(dict(number=number,gid=gid,name=data[off+2:off+30] if flags&1 else None))
    off=8+size;images=[]
    while len(images)<count:
        tag,n=struct.unpack_from('<4sI',data,off);assert off+n+8<=len(data)
        if tag==b'PVRT':
            kind,w,h=struct.unpack_from('<IHH',data,off+8);images.append(dict(kind=kind,w=w,h=h,pixels=data[off+16:off+8+n]))
        off+=n+8
    return flags,rows,images
def morton(x,y,w,h):
    result=0;out=0;bit=1
    while bit<w or bit<h:
        if bit<h:result|=bool(y&bit)<<out;out+=1
        if bit<w:result|=bool(x&bit)<<out;out+=1
        bit*=2
    return result
def color16(v,mode):
    five=lambda x:(x<<3)|(x>>2)
    if mode==0:return bytes([five(v>>10&31),five(v>>5&31),five(v&31),255 if v&0x8000 else 0])
    if mode==1:return bytes([five(v>>11&31),((v>>5&63)<<2)|((v>>5&63)>>4),five(v&31),255])
    if mode==2:return bytes([(v>>8&15)*17,(v>>4&15)*17,(v&15)*17,(v>>12&15)*17])
    raise AssertionError(mode)
def image_oracle(image,outw=64,outh=64,ignore_alpha=False):
    kind,w,h=image['kind'],image['w'],image['h'];fmt=kind&255;category=kind&0xff00;src=image['pixels']
    assert category in [0x100,0xd00,0x900] and fmt in [0,1,2]
    rgba=bytearray()
    for y in range(outh):
        for x in range(outw):
            sx=((2*x+1)*w)//(2*outw);sy=((2*y+1)*h)//(2*outh)
            at=sy*w+sx if category==0x900 else morton(sx,sy,w,h)
            pixel=color16(int.from_bytes(src[2*at:2*at+2],'little'),fmt)
            rgba+=pixel[:3]+b'\xff' if ignore_alpha else pixel
    return bytes(rgba)
