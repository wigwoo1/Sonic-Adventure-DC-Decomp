import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_directory_parent_sector_adjust.dll'))
@Call
def call(*args):raise AssertionError('unexpected external call')
fixtures=[]
for count,present,bank,sector in itertools.product([0,1,2,3,0xffffffff],[0,1],[0,0x200000],[0,1234,0xffffffff]):
 data=bytearray(128);struct.pack_into('<I',data,0,count);struct.pack_into('<I',data,16,sector);struct.pack_into('<I',data,60,0xdeadbeef)
 fixtures.append((0x8c5ffcfe,'sa_file_directory_parent_sector_adjust',data,0,bank,(count,present,sector)))
cases=0
for pc,name,data,alignment,bank,expected in fixtures:
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 offset=0xa00000+alignment;ram[offset:offset+len(data)]=data
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8c000000+offset if expected[1] else 0;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=pc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 fn=getattr(lib,name);fn.argtypes=abi
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 count,present,sector=expected
 wanted=bytearray(data)
 if present and 2<=count<0x80000000:struct.pack_into('<I',wanted,60,sector)
 assert native[offset:offset+len(data)]==wanted
 cases+=1
report={'cases_passed':cases,'scope':'Native directory parent-sector adjustment compared with full retail RAM/CPU/FPU/SR/PR without hooks. Null/present directory, signed counts0/1/2/3/-1, sector values0/1234/ffffffff and banks. Independent complete directory bytes, including copied first-to-second sector only for count>=2.'}
(root/'analysis/file_directory_parent_sector_adjust_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'directory parent-sector adjustment comparisons passed')
