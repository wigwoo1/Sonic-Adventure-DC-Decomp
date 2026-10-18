import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    fn=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);fn.argtypes=abi;return fn
fn=load('sound_stream_decode_schedule.dll','sa_sound_stream_decode_schedule')
division=load('signed_division.dll','sa_signed_division');remainder=load('integer_remainder.dll','sa_integer_remainder')
cases=0;counts=set()
for block,channels,available,offset,remaining,position,unit,bank in itertools.product([16,32],[1,2],[0,31,96],[0,7],[0,65],[0,9],[1,2],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa70000:0xa70100]=bytes([0x66])*256
    for field,value in [(0x54,unit),(0x58,channels),(0x60,block),(0x68,available),(0x70,offset),(0x74,remaining),(0x78,position)]:struct.pack_into('<I',ram,0xa70000+field,value)
    before=bytes(ram[0xa70000:0xa70100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[]
    def hook(address):
        def run(mem,r,sr):expected.append((address,r[4],r[5]));return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address in [0x8c6677e2,0x8c66780e]:actual.append((address,r[4],r[5]));return 0
        target={0x8c10ca5c:division,0x8c10cbb8:remainder}[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca70000;s.r[15]=0x8cf00000;s.pc=0x8c667844;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);special={'macl':0}
    reference(ram,s,{a:hook(a) for a in [0x8c6677e2,0x8c66780e]},fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    def q(n,d):return (abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1)
    rounded=q(position+block-1,block);padding=block-((position+block-1)%block)-1
    capacity=q(available-offset+block-1,block)
    if rounded<capacity and capacity*block-padding+offset<available:capacity+=1
    limit=remaining+(padding if position<remaining else 0)
    chosen=min(q(unit,channels),q(limit,block),rounded,capacity)
    target=0x8c66780e if channels==2 else 0x8c6677e2
    assert actual==[(target,0x8ca70000,chosen&0xffffffff)],(block,channels,available,offset,remaining,position,unit,actual,chosen)
    assert native[0xa70000:0xa70100]==before and regs[15]==0x8cf00000
    counts.add(chosen);cases+=1
(out/'analysis/sound_stream_decode_schedule_verification.json').write_text(json.dumps(dict(cases_passed=cases,selected_counts=sorted(counts),scope='384 full retail RAM/CPU/FPU/SR/PR/MACL scheduler comparisons667844..667900. Native signed division and remainder; only downstream mono/stereo request wrappers modeled and traced. Independent signed arithmetic count-selection oracle, stream immutability and stack checks. Block16/32,channels1/2,availability0/31/96,offset0/7,remaining0/65,position0/9,unit1/2,both banks. Negative/overflow/zero divisors, downstream decoder integration, full scheduling/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream scheduler comparisons passed')
