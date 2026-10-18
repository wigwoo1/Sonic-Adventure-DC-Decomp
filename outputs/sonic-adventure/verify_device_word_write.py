import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_word_write.dll')).sa_device_word_write;fn.argtypes=abi;cases=0
for address,value,bank in itertools.product([0x100,0x101,0x102,0x103,0x1fffff,0x200000,0xffffffff],[0,1,0x1234ffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xb01ffc:0xb02008]=bytes([0xa5])*12;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];words=[0x89abcdef,0x89abcdef]
 def service(t,m,r,events,which):
  assert t==0x8c10d770
  stack=r[15]&0xffffff;extra=struct.unpack('<2I',bytes(m[stack:stack+8]))
  events.append((t,tuple(r[i] for i in range(4,8)),extra))
  at=r[4]&0xffffff;payload=struct.unpack('<I',bytes(m[at:at+4]))[0];words[which]=payload
  events[-1]=events[-1]+(payload,)
  r[0]=3
 hooks={}
 for t in [0x8c10d770]:
  def hook(m,r,sr,t=t):service(t,m,r,expected,0);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  try:service(t,m,r,actual,1)
  except Exception as exc:errors.append(repr(exc));return -9
  return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=address;s.r[5]=value;s.r[15]=0x8cf00000;s.pc=0x8c66ac3c;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected and words[0]==words[1]
 assert r[0]==0 and r[15]==0x8cf00000
 allowed=address<0x200000 or address>=0x80000000
 if allowed:
  assert len(actual)==1 and actual[0][0]==0x8c10d770
  assert actual[0][1][1:]==((0xa0800000+address)&0xffffffff,4,1) and actual[0][2:]==((0,0),value)
  assert words[1]==value
  assert native[0xb01ffc:0xb02008]==bytes([0xa5])*12
 else:assert not actual and native[0xb01ffc:0xb02008]==bytes([0xa5])*12
 cases+=1
report={'cases_passed':cases,'instructions_recovered':25,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/event equality; seven offsets including all lanes, limit and negative signed values, three input words, both FPU banks. Independent device alias, four-byte transfer, stride1 and zero stack arguments, stack payload and untouched RAM guards, return0 and rejected address no-op. Transfer10d770 modeled; actual hardware unverified.'}
(root/'analysis/device_word_write_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'device word write comparisons passed')
