import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_record_initialize.dll')).sa_engine_render_record_initialize;fn.argtypes=abi
copy_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/record_copy_28.dll'))
dispatch=copy_lib.sa_record_copy_dispatch;dispatch.argtypes=abi
copy_28=copy_lib.sa_record_copy_28;copy_28.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=abi
literal=lambda a:struct.unpack_from('<I',binary,a-0x8c010000)[0]
context_global=literal(0x8c666a48)&0xffffff;field_offset=struct.unpack_from('<h',binary,0x666a46-0x10000)[0]
error=literal(0x8c666a4c);targets=[0x8c10d31c,0x8c10cd44,0x8c65acac];cases=0
for present,count,bank in itertools.product([False,True],[0,1,3,8],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,context_global,0x8cb08000 if present else 0)
 ram[0xb08000:0xb09000]=bytes([0xa5])*4096;ram[0xb00000:0xb00100]=bytes([0xcc])*256
 template=bytes((i*19+7)&255 for i in range(28));ram[0x67c334:0x67c350]=template
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(t,m,r,events):
  events.append((t,tuple(r[i] for i in range(7))))
  if t==0x8c10d31c:
   assert r[4]==0x8cb00000 and r[5]==0 and r[6]==28*count
   at=r[4]&0xffffff
   for i in range(r[6]):m[at+i]=0
   r[0]=r[4]
  elif t in targets:r[0]=0
  else:raise AssertionError(hex(t))
 hooks={}
 for target in targets:
  def hook(m,r,sr,target=target):
   if target in [0x8c10cd44,0x8c10d31c]:
    expected.append((target,tuple(r[i] for i in range(7))))
    nested=State();nested.r[:]=r;nested.pc=target;nested.pr=0x8c000100;nested.sr=sr
    reference(m,nested,fpu=f)
    for i in range(16):r[i]=nested.r[i]
    return nested.sr
   service(target,m,r,expected);return sr
  hooks[target]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c10d31c:
   actual.append((t,tuple(r[i] for i in range(7))))
   return fill(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t==0x8c10cd7a:return copy_28(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t==0x8c10cd44:
   actual.append((t,tuple(r[i] for i in range(7))))
   return dispatch(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  try:service(t,m,r,actual)
  except Exception as exc:errors.append(repr(exc));return -9
  return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8cb00000;s.r[5]=count;s.r[15]=0x8cf00000;s.pc=0x8c6669c4;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
 assert r[0]==(0 if present else error)
 if present:
  at=0xb08000+field_offset
  assert struct.unpack_from('<II',native,at)==(0x8cb00000,count)
  assert struct.unpack_from('<II',native,at+12)==(0,0)
  assert native[0xb00000:0xb00100]==template*count+bytes([0xcc])*(256-28*count)
  assert [event[1][4] for event in actual if event[0]==0x8c10cd44]==[28*i for i in range(count)]
 else:assert [event[0] for event in actual]==[0x8c65acac] and actual[0][1][4:6]==(0x8c6669c4,error)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':65,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/event equality for absent/present synthetic context, counts0/1/3/8, both FPU banks. Independent context fields, return/error registration, fill size/tail and 28-byte record initializer offsets. Native fill10d31c connected; only error registrar65acac modeled; native dispatcher10cd44 and 28-byte copy leaf10cd7a connected; independent repeated template payload checked; other jump-table sizes and actual game use unverified.'}
(root/'analysis/engine_render_record_fill_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'record initializer comparisons passed')
