import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for selector,level,bank,capacity in itertools.product([-1,0,1,2,3],[-127,0,127,255],[0,0x200000],[0,1]):
 answer=0
 sr0=0x70000063;entry=0x8c66b66c
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_channel_volume.dll')).sa_sound_channel_volume
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c66b700),0x8ca09000);ram[0xa08001]=0x42
 target=struct.unpack_from('<I',binary,0x66b72c-0x10000)[0];error_target=struct.unpack_from('<I',binary,0x66b734-0x10000)[0]
 packet=lit(0x8c66b70c);expected=bytearray(ram[packet:packet+20]);struct.pack_into('<I',expected,0,0x25)
 expected[4]=ram[lit(0x8c66b710)+(selector&3)]|0x42
 signed_level=ctypes.c_int8(level).value;bias=struct.unpack_from('<h',binary,0x66b6fc-0x10000)[0]
 auxiliary=0x89abcdef
 for active,literals in [(selector in (0,-1),(0x66b714,0x66b718,0x66b71c)),(selector in (1,-1),(0x66b720,0x66b724,0x66b728))]:
  if active:
   for addr,value in zip(literals,((signed_level+bias)&255,auxiliary&255,(ctypes.c_int32(auxiliary).value>>8)&255)):expected[lit(0x8c000000+addr)-packet]=value
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,256 if capacity else 0,0,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=selector&0xffffffff;s.r[6]=level&0xffffffff;s.r[7]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 assert native[packet:packet+20]==expected
 assert struct.unpack_from('<3I',native,ring+20)==((256,0,0) if capacity else (1,1,0))
 assert struct.unpack_from('<I',native,0xa10014)[0]==(0 if capacity else 1)
 if not capacity:assert native[0xa10000:0xa10014]==expected
 else:assert native[0xa10000:0xa1001c]==bytes(28)
 cases+=1
(out/'analysis/sound_channel_volume_native_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native72-instruction per-channel command25 builder; selectors-1/0/1/2/3, signed byte levels,both banks and empty/full256-entry queue. Full retail state; independent20-byte command template/identifier/channel fields/auxiliary arithmetic shift/untouched bytes. Commit/loaded copy dispatch/copy20 native; independent request/tag/cursors checked and full-queue preserves pool. Error services modeled; audio unverified.'),indent=2)+'\n')
print(cases,'channel volume native enqueue comparisons passed')
