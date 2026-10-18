import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f
create=load('sound_stream_create.dll','sa_sound_stream_create');destroy=load('sound_stream_destroy.dll','sa_sound_stream_destroy');fill=load('byte_fill.dll','sa_byte_fill');cases=0
b=binary;lower={}
import math
from fractions import Fraction
from sh4_binary32 import multiply,multiply_add,divide,add_sub
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def val(word):
    if word==0:return Fraction(0)
    exponent=(word>>23)-150; significand=(word&0x7fffff)|0x800000
    return Fraction(significand)*Fraction(2)**exponent
def sqrt_service(word,fp):
    assert word&0xf0ff==0xf06d
    bank=(fp.fpscr>>21)&1; n=(word>>8)&15; x=val(fp.banks[bank][n])
    candidate=raw(math.sqrt(float(x)))
    while val(candidate)**2>x:candidate-=1
    while val(candidate+1)**2<=x:candidate+=1
    inexact=val(candidate)**2!=x
    if inexact:
        midpoint=(val(candidate)+val(candidate+1))/2
        if x>midpoint**2 or (x==midpoint**2 and candidate&1):candidate+=1
    fp.banks[bank][n]=candidate
    fp.fpscr=(fp.fpscr&~0x3f000)|(0x1004 if inexact else 0)
def trig_oracle(angle):
    x=angle&0x7fffffff;index=int(struct.unpack('<f',struct.pack('<I',divide(x,0x3f490fdb)[0]))[0])
    remainder=add_sub(x,multiply(raw(float(index)),0x3f490fdb)[0],True)[0];octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',b,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    return struct.unpack('<f',struct.pack('<I',answer))[0]
for address,stem,name in [(0x8c644be6,'file_audio_configuration_forward','sa_file_audio_configuration_forward'),(0x8c65adc0,'audio_filter_coefficients','sa_audio_filter_coefficients'),(0x8c10f90c,'math_result_error_wrapper','sa_math_result_error_wrapper'),(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi

handle_pool=struct.unpack_from('<I',binary,0x8c644c9c-0x8c010000)[0]
lower[0x8c644b6e]=load('file_decode_handle_allocate.dll','sa_file_decode_handle_allocate');lower[0x8c644bfe]=load('file_decode_handle_release.dll','sa_file_decode_handle_release')
pool=0x8c8d61ac;size=0xac
for free,handle_free,bank in itertools.product(range(9),range(9),[0,1]):
    handle=handle_pool+handle_free*52 if handle_free<8 else 0
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[pool&0xffffff:(pool&0xffffff)+size*8]=bytes([0x55])*(size*8)
    for index in range(8):struct.pack_into('<H',ram,(pool&0xffffff)+size*index,0 if index==free else 1)
    hs=handle_pool&0xffffff
    ram[hs:hs+52*8]=bytes([0x77])*(52*8)
    for index in range(8):struct.pack_into('<I',ram,hs+52*index,0 if index==handle_free else 1)
    handles_before=bytes(ram[hs:hs+52*8])
    before=bytes(ram[pool&0xffffff:(pool&0xffffff)+size*8]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target={0x8c10d31c:fill,0x8c667440:destroy}.get(address) or lower[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];args=[0x8ca10000,512,0x8ca20000,65536];s.r[4:8]=args;s.r[15]=0x8cf00000;s.pc=0x8c6673b4;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert create(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    wanted=bytearray(before)
    if free<8:
        obj=bytearray(size)
        if handle:
            struct.pack_into('<H',obj,0,1);struct.pack_into('<I',obj,8,handle);struct.pack_into('<4I',obj,0x38,*args)
            struct.pack_into('<4I',obj,0x88,0x8c667384,pool+free*size,0x8c6673a2,pool+free*size)
        wanted[free*size:(free+1)*size]=obj
    assert native[pool&0xffffff:(pool&0xffffff)+size*8]==wanted
    assert regs[0]==(pool+free*size if free<8 and handle else 0) and regs[15]==0x8cf00000
    assert actual==expected==[]
    wanted_handles=bytearray(handles_before)
    if free<8 and handle:
        obj=bytearray(52);struct.pack_into('<2I',obj,0,1,handle_free)
        # The connected coefficient path is independently checked by its
        # dedicated pool verifier; here preserve and verify all other bytes.
        coefficient_bytes=native[hs+handle_free*52+48:hs+handle_free*52+52]
        obj[48:52]=coefficient_bytes
        wanted_handles[handle_free*52:(handle_free+1)*52]=obj
    assert native[hs:hs+52*8]==wanted_handles
    stream_pointer=regs[0]
    allocated_streams=bytes(wanted);allocated_handles=bytes(wanted_handles)
    # Destroy a successfully allocated stream, or pass null after a failure.
    s.r[4]=stream_pointer;regs[4]=stream_pointer;s.pc=0x8c667440
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert destroy(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    if stream_pointer:
        wanted[free*size:(free+1)*size]=bytes(size)
        wanted_handles[handle_free*52:(handle_free+1)*52]=bytes(52)
    assert native[pool&0xffffff:(pool&0xffffff)+size*8]==wanted
    assert native[hs:hs+52*8]==wanted_handles
    # Null destruction must preserve both entire pools and every RAM byte.
    prior_ram=bytes(native);s.r[4]=0;regs[4]=0;s.pc=0x8c667440
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert destroy(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    # SH4 prologue/epilogue stack stores are the sole permitted RAM changes.
    assert native[:0xeffff8]==prior_ram[:0xeffff8] and native[0xf00000:]==prior_ram[0xf00000:]
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    for index in range(4,8):s.r[index]=args[index-4];regs[index]=s.r[index]
    s.pc=0x8c6673b4
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert create(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    assert native[pool&0xffffff:(pool&0xffffff)+size*8]==allocated_streams
    assert native[hs:hs+52*8]==allocated_handles
    assert regs[0]==stream_pointer and regs[15]==0x8cf00000
    cases+=1
(p/'analysis/stream_pool_reuse_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='162 persistent construction/destruction/null-destruction/reallocation sequences with full retail RAM/CPU/FPU/SR/PR/MACL stream constructor6673b4..667440 comparisons. First free slots0..7 and exhausted pool,every free decode-handle slot and exhausted handle pool,both FPU banks. Native zero-fill and destructor667440..667464 on failure; handle allocator/release and coefficient math native; no modeled calls. Independent entire eight-slot pool,callback/default userdata,arguments,active flag,handle,failure clearing,return pointer/stack checked. All unchanged decode-pool entries and handle initialization bytes checked; coefficient fields compared against retail instructions here and independently verified in file_handle_native_coefficients_verification.json. Successful destruction independently clears the complete selected stream and decode handle; null destruction leaves all RAM outside prologue stack writes unchanged; reallocation restores identical objects and selects the same freed slots. Failed/exhausted cases also repeat. Full playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'native stream pool reuse sequences passed')
