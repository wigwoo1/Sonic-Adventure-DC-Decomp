import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
specs=[(0x8c645348,'sa_file_name_upper'),(0x8c6453c0,'sa_file_name_equal'),(0x8c6453e2,'sa_file_name_equal_folded'),(0x8c645442,'sa_file_name_checksum'),(0x8c6455ce,'sa_file_filename_lookup')]
functions={a:getattr(lib,n) for a,n in specs}
for fn in functions.values():fn.argtypes=abi
cases=0
for query,mode,count,bank in itertools.product([b'E_00B7.MLT',b'e_00b7.mlt',b'OTHER.BIN',b'MISSING',b''],[0,1],[0,1,3],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 names=[b'E_00B7.MLT',b'OTHER.BIN',b'']
 # The retail checksum starts at the signed literal, then sums uppercase bytes.
 seed=struct.unpack_from('<h',binary,0x64554e-0x10000)[0]
 def checksum(name):return ((seed+sum(name.upper()))&0xffffffff)>>2&255
 struct.pack_into('<I',ram,0xa02000,count)
 for i,name in enumerate(names):
  offset=0xa02010+i*44;ram[offset+9]=checksum(name);ram[offset+10:offset+11+len(name)]=name+b'\0'
 ram[0xb00000:0xb00001+len(query)]=query+b'\0'
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8cb00000;s.r[5]=0x8ca02000;s.r[6]=mode;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6455ce
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,target,m,fu,status,regs,ret):
  return functions[target](m,fu,status,regs,ret,ctypes.byref(fpul),ctypes.byref(macl),None,call) if target in functions else -9
 reference(ram,s,fpu=f)
 assert functions[0x8c6455ce](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 matches=[i+1 for i,name in enumerate(names[:count]) if (query==name if mode==1 else query.upper()==name.upper())]
 assert r[0]==(matches[0] if matches else 0)
 cases+=1
report={'cases_passed':cases,'scope':'Native directory lookup, checksum, ASCII uppercase and exact/folded equality against full retail RAM/CPU/FPU/SR/PR. No call hooks; absent/first/second/empty names, case variants, both modes, empty/partial/full directory and FPU banks. Independent one-based match result.'}
(root/'analysis/file_filename_lookup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'native filename lookup comparisons passed')
