import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_volume_helpers.dll'))
@Call
def call(*args):raise AssertionError('unexpected external call')
fixtures=[]
for value,alignment,bank in itertools.product([0,1,0x12345678,0x7fffffff,0x80000000,0xffffffff,0x80ff0180],range(4),[0,0x200000]):fixtures.append((0x8c5ff9c0,'sa_file_volume_word_get',struct.pack('<I',value),alignment,bank,value))
for corrupt,alignment,bank in itertools.product(range(-1,5),range(4),[0,0x200000]):
 data=bytearray(b'\x01CD001\x01')
 if corrupt>=0:data[1+corrupt]^=0x80
 fixtures.append((0x8c5ff9e6,'sa_file_volume_signature_check',data,alignment,bank,int(corrupt==-1)))
cases=0
for pc,name,data,alignment,bank,expected in fixtures:
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 offset=0xa00000+alignment;ram[offset:offset+len(data)]=data
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8c000000+offset;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=pc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 fn=getattr(lib,name);fn.argtypes=abi
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert r[0]==expected
 cases+=1
report={'cases_passed':cases,'scope':'Native little-endian volume word decoder and CD001 signature checker against full retail RAM/CPU/FPU/SR/PR without hooks. All byte alignments, boundary/high-byte words, each signature byte corrupted, valid signature and both FPU banks. Independent results and complete RAM preservation; volume mount integration pending.'}
(root/'analysis/file_volume_helpers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'volume helper comparisons passed')
