import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    fn=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);fn.argtypes=abi;return fn
fn=load('sound_stream_decode_complete.dll','sa_sound_stream_decode_complete');division=load('signed_division.dll','sa_signed_division');remainder=load('integer_remainder.dll','sa_integer_remainder')
cases=0
for block,channels,decoded,offset,position,capacity,bank in itertools.product([16,32],[1,2],[0,1,4],[0,7],[0,9,64],[8,128],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa70000:0xa70100]=bytes([0x66])*256
    for field,value in [(0x8,0x8ca60000),(0x40,capacity),(0x44,128),(0x58,channels),(0x5c,18),(0x60,block),(0x64,0x8ca30000),(0x70,offset),(0x78,position)]:struct.pack_into('<I',ram,0xa70000+field,value)
    before=bytes(ram[0xa70000:0xa70100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(address,r,trace):
        if address==0x8c644c8c:r[0]=decoded
        else:trace.append((address,tuple(r[i] for i in range(4,8))))
    def hook(address):
        def run(mem,r,sr):service(address,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address in [0x8c644c8c,0x8c66792c,0x8c667960]:service(address,r,actual);return 0
        target={0x8c10ca5c:division,0x8c10cbb8:remainder}[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca70000;s.r[15]=0x8cf00000;s.pc=0x8c667978;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);special={'macl':0}
    reference(ram,s,{a:hook(a) for a in [0x8c644c8c,0x8c66792c,0x8c667960]},fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    rounded=(position+block-1)//block;padding=block-((position+block-1)%block)-1
    samples=block*decoded//channels-(padding if rounded<=decoded else 0)
    wanted=bytearray(before);struct.pack_into('<2I',wanted,0xa0,samples&0xffffffff,18*decoded)
    trace=[]
    if offset+samples>=capacity:trace=[(0x8c66792c if channels==2 else 0x8c667960,(0x8ca30000,capacity,128,(offset+samples-capacity)&0xffffffff))]
    assert native[0xa70000:0xa70100]==wanted and actual==trace and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_stream_decode_complete_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='288 full retail RAM/CPU/FPU/SR/PR/MACL stream completion comparisons667978..667a4a. Native signed division/remainder; result getter and downstream compaction modeled. Independent sample-count/padding arithmetic, byte-count output+a4, sample-count output+a0, complete256-byte stream oracle and mono/stereo compaction argument trace. Block16/32,channels1/2,decoded0/1/4,offset0/7,position0/9/64,capacity8/128,both banks. Native getter/compaction integration, overflow/zero divisors, continuous playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream completion comparisons passed')
