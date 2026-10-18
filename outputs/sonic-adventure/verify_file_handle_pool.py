import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f
allocate=load('file_decode_handle_allocate.dll','sa_file_decode_handle_allocate');release=load('file_decode_handle_release.dll','sa_file_decode_handle_release');fill=load('byte_fill.dll','sa_byte_fill')
pool=struct.unpack_from('<I',b,0x8c644c9c-base)[0];size=52;cases=0
for free,bank in itertools.product(range(9),[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;start=pool&0xffffff
    ram[start:start+size*8]=bytes([0x55])*(size*8)
    for index in range(8):struct.pack_into('<I',ram,start+size*index,0 if index==free else 1)
    before=bytes(ram[start:start+size*8]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(address,mem,r,trace):
        assert address==0x8c65adc0
        trace.append(tuple(r[i] for i in range(4,8)))
        assert r[4]==struct.unpack_from('<h',b,0x8c644c9a-base)[0] and r[5]==struct.unpack_from('<I',b,0x8c644ca4-base)[0]
        assert r[6]==pool+free*size+48 and r[7]==r[6]+2
        for offset,value in [(r[6]&0xffffff,123),(r[7]&0xffffff,(-456)&65535)]:
            mem[offset]=value&255;mem[offset+1]=value>>8
    def hook(mem,r,sr):service(0x8c65adc0,mem,r,expected);return sr
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address==0x8c10d31c:return fill(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s.pc=0x8c644b6e;reference(ram,s,{0x8c65adc0:hook},fpu=f,special=special)
    assert allocate(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    wanted=bytearray(before)
    if free<8:
        obj=bytearray(size);struct.pack_into('<2I',obj,0,1,free);struct.pack_into('<2h',obj,48,123,-456);wanted[free*size:(free+1)*size]=obj
    assert native[start:start+size*8]==wanted and regs[0]==(pool+free*size if free<8 else 0)
    handle=regs[0];regs[4]=handle;s.r[4]=handle;s.pc=0x8c644bfe
    reference(ram,s,fpu=f,special=special)
    assert release(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    if free<8:wanted[free*size:(free+1)*size]=bytes(size)
    assert native[start:start+size*8]==wanted and regs[15]==0x8cf00000
    cases+=1
(p/'analysis/file_handle_pool_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='18 persistent allocation/release sequences: all eight free slots and exhausted decode-handle pool,both FPU banks. Full retail RAM/CPU/FPU/SR/PR/MACL compared. Independent 52-byte entry initialization,index,active flag,predictor/history fields,entire pool preservation,returned pointer and release clearing/null no-op. Byte fill native; coefficient generation65adc0 modeled with argument assertions and distinct signed results. Native coefficient integration and concurrency unverified.'),indent=2)+'\n');print(cases,'file handle pool sequences passed')
