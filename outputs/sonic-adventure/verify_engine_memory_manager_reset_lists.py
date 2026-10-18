import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_memory_manager_reset.dll')).sa_engine_memory_manager_reset;fn.argtypes=abi
cases=0
for lengths,destroy,bank,initial_sr in itertools.product([(0,0,0,0,0),(1,1,1,1,1),(3,3,3,3,3),(2,0,1,3,2)],[False,True],[0,0x200000],[0x70000063,0x700000f3]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0x8a83e4:0x8c03e4]=bytes([0xa5])*0x18000
 ram[0x67c2e0:0x67c308]=bytes([0xcc])*40
 desired=[];nodes=[]
 for group,(offset,length) in enumerate(zip([0x67c2e8,0x67c2e0,0x67c2fc,0x67c2f4,0x67c308],lengths)):
  head=0x8cb00000+group*0x1000 if length else 0;struct.pack_into('<I',ram,offset,head)
  for i in range(length):
   address=head+i*32;node_offset=address&0xffffff;ram[node_offset:node_offset+32]=bytes([0x77])*32
   link=4 if group==4 else 8;next_address=address+32 if i+1<length else 0
   struct.pack_into('<I',ram,node_offset+link,next_address)
   target=0x8c665440 if group==4 else 0x8c664aec
   desired.append((target,address));nodes.append((node_offset,link))
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);events=[];actual=[];errors=[]
 def release(target,m,r,log):
  log.append((target,r[4]));link=4 if target==0x8c665440 else 8
  if destroy:
   offset=(r[4]&0xffffff)+link
   for i in range(4):m[offset+i]=0
  r[0]=0x1234
 hooks={}
 for target in [0x8c664aec,0x8c665440]:
  def hook(m,r,sr,target=target):release(target,m,r,events);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target not in hooks:errors.append(target);return -9
  release(target,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c663080;s.pr=0x8c000100;s.sr=initial_sr
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert actual==desired and r[0]==0
 assert native[0x67c2e0:0x67c30c]==bytes(44)
 for offset in range(0x8a83e4,0x8c03e4,24):assert native[offset:offset+24]==bytes(2)+bytes([0xa5])*22
 for offset,link in nodes:
  assert native[offset:offset+link]==bytes([0x77])*link and native[offset+link+4:offset+32]==bytes([0x77])*(28-link)
  if destroy:assert native[offset+link:offset+link+4]==bytes(4)
 assert r[13]==0x2234000d and r[14]==0x2234000e and r[15]==0x8cf00000
 cases+=1
report={'cases_passed':cases,'scope':'Full retail RAM/CPU/FPU/SR/PR equality across four shapes of five lists, two SR/FPU states and release callbacks preserving or destroying next links. Independent exact release order, all4096 descriptor flags cleared with22-byte tails preserved, manager44-byte cleanup and node canaries. Release services664aec/665440 modeled; real allocator release not verified.'}
(root/'analysis/engine_memory_manager_reset_lists_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'live-list manager reset comparisons passed')
