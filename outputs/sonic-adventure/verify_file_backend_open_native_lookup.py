import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_backend_open.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cleanup=lib.sa_file_backend_open;cleanup.argtypes=abi
functions={}
for address,name,dll in [(0x8c600464,'sa_file_handle_open_initialize','file_handle_open_initialize'),(0x8c5ffa08,'sa_file_error_set','file_error_set')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
lookup_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll'))
for address,name in [(0x8c645348,'sa_file_name_upper'),(0x8c6453c0,'sa_file_name_equal'),(0x8c6453e2,'sa_file_name_equal_folded'),(0x8c645442,'sa_file_name_checksum'),(0x8c6455ce,'sa_file_filename_lookup')]:
 helper=getattr(lookup_lib,name);helper.argtypes=abi;functions[address]=helper
cases=0
for match,pattern,explicit,bank in itertools.product([0,1,2],[[0,1,1],[1,1,0],[1,1,1]],[0,1],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x673414,0x8ca01000)
 for offset,value in [(0,3),(0x10,0x8ca02000),(0x18,0x8ca03000),(0x30,0xdeadbeef)]:struct.pack_into('<I',ram,0xa01000+offset,value)
 for index in range(2):struct.pack_into('<II',ram,0xa02010+index*44,100+index*10,13120+index*1024)
 for index,active in enumerate(pattern):
  ram[0xa03000+index*84:0xa03000+(index+1)*84]=bytes([0x55])*84
  struct.pack_into('<I',ram,0xa03000+index*84,0x8ca01000);struct.pack_into('<H',ram,0xa0304c+index*84,active)
 names=[b'FIRST.MLT',b'SECOND.MLT']
 seed=struct.unpack_from('<h',binary,0x64554e-0x10000)[0]
 struct.pack_into('<I',ram,0xa02000,2)
 for i,name in enumerate(names):
  o=0xa02010+i*44;ram[o+9]=((seed+sum(name.upper()))&0xffffffff)>>2&255;ram[o+10:o+11+len(name)]=name+b'\0'
 query=names[match-1].lower() if match else b'ABSENT.MLT'
 ram[0xb00000:0xb00001+len(query)]=query+b'\0'
 before=bytes(ram[0xa03000:0xa030fc]);native_lookup=[];reference_lookup=[]
 def hook(m,regs,status):reference_lookup.append(tuple(regs[i] for i in [4,5,6]));regs[0]=match;return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  return functions[target](m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call) if target in functions else -9
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8cb00000;s.r[5]=0x8ca02000 if explicit else 0;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6004d4
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 s.pc=0x8c6004d4
 reference(ram,s,fpu=f,special=special,hooks=None)
 assert cleanup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 selected=pattern.index(0) if 0 in pattern else None
 success=match!=0 and selected is not None
 assert r[0]==(0x8ca03000+selected*84 if success else 0)
 assert struct.unpack_from('<I',native,0xa01030)[0]==(0 if success else 0xfffffff8 if match==0 else 0xfffffff7)
 for index in range(3):
  if not success or index!=selected:assert native[0xa03000+index*84:0xa03000+(index+1)*84]==before[index*84:(index+1)*84]
 if success:
  offset=0xa03000+selected*84
  assert struct.unpack_from('<4I',native,offset+4)==(match,250+10*(match-1),13120+1024*(match-1),(13120+1024*(match-1)+2047)//2048)
  assert struct.unpack_from('<H',native,offset+0x4c)[0]==1
 cases+=1
(root/'analysis/file_backend_open_native_lookup_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native backend open/handle initialization/error setter against full retail RAM/CPU/FPU/SR/PR/MACL. Native filename search selects absent/first/second directory record, default/explicit directory, first/last free/full pool, banks. Independent selected handle, record fields, error0/-8/-9 and unselected-handle preservation. No call hooks; native checksum, case folding and comparisons connected.'},indent=2)+'\n')
print(cases,'backend file open native lookup comparisons passed')