import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_directory_parser.dll'));names=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll'))
fn=lib.sa_file_directory_load;fn.argtypes=abi
helpers={0x8c645468:lib.sa_file_directory_record_parse,0x8c64535c:lib.sa_file_directory_bytes_copy,0x8c645442:names.sa_file_name_checksum,0x8c645348:names.sa_file_name_upper}
for helper in helpers.values():helper.argtypes=abi
sector=bytearray(2048)
for i,name in enumerate([b'FIRST.MLT;1',b'SECOND.MLT;1']):
 o=i*64;sector[o]=64;struct.pack_into('<I',sector,o+2,100+i);struct.pack_into('<I',sector,o+10,2048);sector[o+32]=len(name);sector[o+33:o+33+len(name)]=name
cases=0
for capacity,cached,error,bank in itertools.product([1,2,3],[0,1],[0,0xffffffef],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<4I',ram,0xb00000,99,capacity,123 if cached else 0,0)
 events=[];actual=[]
 def service(m,regs,log):
  log.append(tuple(regs[i] for i in range(4,8)));assert regs[4]==123 and regs[5]==1 and regs[6]==0x8ca00000
  if not error:
   for i,v in enumerate(sector):m[0xa00000+i]=v
  regs[0]=error
 def hook(m,regs,status):service(m,regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c5ffc98:service(m,regs,actual);return 0
  return helpers[target](m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=123;s.r[6]=0x8cb00000;s.r[7]=2048;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6454fe
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8c5ffc98:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 result=0 if cached else error if error else 0xfffffff4 if capacity==1 else 0
 assert r[0]==result and len(actual)==int(not cached)
 assert struct.unpack_from('<I',native,0xb00008)[0]==(0 if result else 123)
 if not cached and not error:
  assert struct.unpack_from('<I',native,0xb00000)[0]==min(capacity,2)
  for i,name in enumerate([b'FIRST.MLT',b'SECOND.MLT'][:capacity]):
   o=0xb00010+i*44;assert struct.unpack_from('<II',native,o)==(100+i,2048);assert native[o+10:o+11+len(name)]==name+b'\0'
 cases+=1
report={'cases_passed':cases,'scope':'Native directory loader/record parser/copy/checksum/uppercase against full retail RAM/CPU/FPU/SR/PR. Single modeled sector-read callback; capacities1/2/3, cached reuse, success/read error and banks. Independent result, cache invalidation, count, decoded fields and filenames. Actual disc read and multi-sector loading pending.'}
(root/'analysis/file_directory_load_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'directory loader comparisons passed')
