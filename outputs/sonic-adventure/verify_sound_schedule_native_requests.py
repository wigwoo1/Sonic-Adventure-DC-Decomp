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
mono=load('sound_stream_mono_request.dll','sa_sound_stream_mono_request');stereo=load('sound_stream_stereo_request.dll','sa_sound_stream_stereo_request')
mono_setup=load('file_decode_mono_request.dll','sa_file_decode_mono_request');stereo_setup=load('file_decode_stereo_request.dll','sa_file_decode_stereo_request');activate=load('file_decode_request_activate.dll','sa_file_decode_request_activate')
cases=0;counts=set()
for block,channels,available,offset,remaining,position,unit,bank,phase in itertools.product([16,32],[1,2],[0,31,96],[0,7],[0,65],[0,9],[1,2],[0,1],[0,2]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa70000:0xa70100]=bytes([0x66])*256
    for field,value in [(0x54,unit),(0x58,channels),(0x60,block),(0x68,available),(0x70,offset),(0x74,remaining),(0x78,position)]:struct.pack_into('<I',ram,0xa70000+field,value)
    struct.pack_into('<I',ram,0xa70008,0x8ca60000);struct.pack_into('<I',ram,0xa70050,0x8ca10000);struct.pack_into('<I',ram,0xa70064,0x8ca30000);struct.pack_into('<I',ram,0xa7006c,128)
    ram[0xa60000:0xa60040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa6000c,phase);handle_before=bytes(ram[0xa60000:0xa60040])
    before=bytes(ram[0xa70000:0xa70100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[]
    def hook(address):
        def run(mem,r,sr):expected.append((address,r[4],r[5]));return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address in [0x8c6677e2,0x8c66780e]:actual.append((address,r[4],r[5]))
        target={0x8c6677e2:mono,0x8c66780e:stereo,0x8c644c1a:mono_setup,0x8c644c38:stereo_setup,0x8c644c56:activate}.get(address)
        if target:return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        target={0x8c10ca5c:division,0x8c10cbb8:remainder}[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca70000;s.r[15]=0x8cf00000;s.pc=0x8c667844;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);special={'macl':0}
    reference(ram,s,fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    def q(n,d):return (abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1)
    rounded=q(position+block-1,block);padding=block-((position+block-1)%block)-1
    capacity=q(available-offset+block-1,block)
    if rounded<capacity and capacity*block-padding+offset<available:capacity+=1
    limit=remaining+(padding if position<remaining else 0)
    chosen=min(q(unit,channels),q(limit,block),rounded,capacity)
    target=0x8c66780e if channels==2 else 0x8c6677e2
    assert actual==[(target,0x8ca70000,chosen&0xffffffff)],(block,channels,available,offset,remaining,position,unit,actual,chosen)
    assert native[0xa70000:0xa70100]==before and regs[15]==0x8cf00000
    wanted=bytearray(handle_before)
    if phase==0:
        left=0x8ca30000+offset*2;right=left+256 if channels==2 else 0
        struct.pack_into('<2I',wanted,12,1,0);struct.pack_into('<5I',wanted,20,channels,0x8ca10000,(chosen*channels)&0xffffffff,left,right)
    assert native[0xa60000:0xa60040]==wanted
    counts.add(chosen);cases+=1
(out/'analysis/sound_schedule_native_requests_verification.json').write_text(json.dumps(dict(cases_passed=cases,selected_counts=sorted(counts),scope='768 full retail RAM/CPU/FPU/SR/PR/MACL connected scheduler comparisons with native division/remainder, mono/stereo stream wrappers, request setup and activation; no modeled calls. Same384 scheduling fixtures crossed with idle/busy decoder handles. Independent signed count oracle, wrapper selection trace, output offsets, doubled stereo block count, full64-byte handle oracle, stream immutability and stack checks. Actual sample decoding, negative/overflow/zero divisors, continuous playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'native scheduled request comparisons passed')
