import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_record_complete.dll')).sa_engine_render_record_complete;fn.argtypes=abi;cases=0
byte_write=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_byte_write.dll')).sa_device_byte_write;byte_write.argtypes=abi
for count,address,result,bank in itertools.product([0,1,2,4,0xffffffff],[0x100,0x101,0x102,0x103,0x200000,0x12345678],[0,3],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x67c1d0,0x8cb08000)
 ram[0xb08000:0xb08400]=bytes([0xa5])*1024;ram[0xb00000:0xb00080]=bytes([0x99])*128
 for offset,value in [(344,0x8cb00000),(348,count),(352,7),(444,address)]:struct.pack_into('<I',ram,0xb08000+offset,value)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 words=[0x89abcdef,0x89abcdef]
 def service(t,m,r,events,which):
  if t==0x8c66910c:
   at=r[5]&0xffffff
   for i in range(4):m[at+i]=(words[which]>>(8*i))&255
   events.append((t,r[4]));r[0]=result
  elif t==0x8c66ac3c:events.append((t,r[4],r[5]));words[which]=r[5];r[0]=result
  else:raise AssertionError(hex(t))
 hooks={}
 for t in [0x8c66910c,0x8c66ac3c]:
  def hook(m,r,sr,t=t):service(t,m,r,expected,0);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c669144:return byte_write(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  try:service(t,m,r,actual,1)
  except Exception as exc:errors.append(repr(exc));return -9
  return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c666ad0;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected and words[0]==words[1]
 if address<0x200000:
  shift=(address&3)*8;word=(0x89abcdef&~(255<<shift))|(1<<shift)
  assert actual==[(0x8c66910c,address&0xfffffffc),(0x8c66ac3c,address&0xfffffffc,word)] and words[1]==word
 else:assert not actual and words[1]==0x89abcdef
 assert struct.unpack_from('<II',native,0xb08000+348)==(0,0)
 wanted=bytearray([0x99])*128
 for i in range(0 if count==0xffffffff else count):wanted[16*i]=0
 assert native[0xb00000:0xb00080]==wanted
 assert r[15]==0x8cf00000 and r[14]==0x2234000e
 cases+=1
report={'cases_passed':cases,'instructions_recovered':34,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/event equality; counts0/1/2/4 and negative signed count, word-service returns0/3, all four byte lanes and rejected addresses, both FPU banks. Independent device byte update/read-write order with other bytes preserved, first-byte clearing at16-byte stride with remaining bytes unchanged, count/active reset and preserved stack/register. Byte-write669144 native; read66910c/write66ac3c modeled; real device completion unverified.'}
(root/'analysis/engine_render_record_complete_byte_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'record completion comparisons passed')
