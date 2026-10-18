import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_byte_write.dll')).sa_device_byte_write;fn.argtypes=abi;cases=0
for address,value,bank in itertools.product([0x100,0x101,0x102,0x103,0x1fffff,0x200000,0xffffffff],[0,1,0x1234ffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];words=[0x89abcdef,0x89abcdef]
 def service(t,m,r,events,which):
  if t==0x8c66910c:
   assert r[4]==address&0xfffffffc
   at=r[5]&0xffffff
   for i in range(4):m[at+i]=(words[which]>>(8*i))&255
   events.append((t,r[4]));r[0]=0
  elif t==0x8c66ac3c:events.append((t,r[4],r[5]));words[which]=r[5];r[0]=3
  else:raise AssertionError(hex(t))
 hooks={}
 for t in [0x8c66910c,0x8c66ac3c]:
  def hook(m,r,sr,t=t):service(t,m,r,expected,0);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  try:service(t,m,r,actual,1)
  except Exception as exc:errors.append(repr(exc));return -9
  return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=address;s.r[5]=value;s.r[15]=0x8cf00000;s.pc=0x8c669144;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected and words[0]==words[1]
 assert r[0]==0 and r[15]==0x8cf00000
 allowed=address<0x200000 or address>=0x80000000
 if allowed:
  shift=(address&3)*8;wanted=(0x89abcdef&~(255<<shift))|((value&255)<<shift)
  assert words[1]==wanted and actual==[(0x8c66910c,address&0xfffffffc),(0x8c66ac3c,address&0xfffffffc,wanted)]
 else:assert not actual and words[1]==0x89abcdef
 cases+=1
report={'cases_passed':cases,'instructions_recovered':33,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/events equality; all byte lanes, limit-1/limit and signed-negative address, three values and both FPU banks. Independent aligned read/write, low-byte insertion with other bytes preserved and signed upper-bound rejection. Read66910c/write66ac3c modeled; real hardware unverified.'}
(root/'analysis/device_byte_write_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'device byte write comparisons passed')
