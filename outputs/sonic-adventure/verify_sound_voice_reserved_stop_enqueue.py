import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_dispatch.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
helpers={}
for filename,entries in [('sound_command_type2.dll',[(0x8c643e08,'sa_sound_command_type2')]),('sound_request_commit.dll',[(0x8c65a464,'sa_sound_request_commit'),(0x8c10cd82,'sa_sound_request_copy20')]),('engine_frame_initialize_native.dll',[(0x8c10cd44,'sa_engine_record_copy_loaded_dispatch')])]:
 owner=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/filename))
 for address,name in entries:helpers[address]=getattr(owner,name)
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
cases=0
for slot,bank,identifier in itertools.product([32,33,34,35,36],[0,0x200000],[0,127,255]):
 entry=0x8c093b70;name='sa_sound_voice_stop_dispatch'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 mapping=lit(0x8c093bb4)
 for index in range(2,7):struct.pack_into('<I',ram,mapping+index*4,0x8ca08000+index*8)
 for index in range(2,7):ram[0xa08000+index*8+1]=identifier
 struct.pack_into('<I',ram,lit(0x8c643e4c),0x8ca09000)
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,0,0,0)
 request_base=lit(0x8c643e58);expected_request=bytearray(ram[request_base:request_base+20]);struct.pack_into('<I',expected_request,0,2);expected_request[4]=identifier
 for helper in helpers.values():helper.argtypes=fn.argtypes
 expected_target=struct.unpack_from('<I',binary,0x093bc0-0x10000)[0] if slot>=32 else 0x8c0933ba
 expected_argument=0x8ca08000+(slot-30)*8 if slot>=32 else slot&0xffffffff
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,tuple(r[i] for i in range(4,8))));r[0]=0;return 0
 hooks={}
 for address in addresses:
  if address in (entry,*helpers):continue
  def hook(m,r,sr,address=address):events.append((address,tuple(r[i] for i in range(4,8))));r[0]=0;return sr
  hooks[address]=hook
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot&0xffffffff;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 assert native[0xa10000:0xa10014]==expected_request and struct.unpack_from('<I',native,0xa10014)[0]==1
 assert struct.unpack_from('<2I',native,ring+20)==(1,1)
 cases+=1
(out/'analysis/sound_voice_reserved_stop_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native stop dispatcher routes reserved slots32..36 to existing native type2 builder643e08 and request commit/copy; identifiers0/127/255,both FPU banks. Full retail state/service trace and independent20-byte request/id/tag/count/write checks. Remaining diagnostic callbacks modeled; no consumer/audio executed.'),indent=2)+'\n')
print(cases,'reserved stop enqueue comparisons passed')
