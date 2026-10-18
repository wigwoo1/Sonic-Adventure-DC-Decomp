import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_field_word_read.dll')).sa_sound_field_word_read;fn.argtypes=abi;cases=0
transfer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;transfer.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
for address,value,mode,bank in itertools.product([0x100,0x101,0x102,0x103,0x1fffff,0x200000,0xffffffff],[0,1,0x1234ffff],[0,1],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xb01ffc:0xb02008]=bytes([0xa5])*12;struct.pack_into('<I',ram,0x5fbb14,0x8cb08000);struct.pack_into('<I',ram,0xb080d4,mode);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];words=[0x89abcdef,0x89abcdef]
 mmio={(0xa0800000+address)&0xffffffff:value};reads=[]
 @Read
 def read(ctx,a,n):
  if a not in mmio or n!=4:errors.append(('read',hex(a),n));return 0
  reads.append(a);return mmio[a]
 @Write
 def write(ctx,a,v,n):errors.append(('unexpected write',hex(a),n))
 io=IO(None,read,write)
 def service(t,m,r,events,which):
  assert t in [0x8c10ee9c,0x8c10eee0,0x8c10ee20]
  events.append((t,tuple(r[i] for i in range(4,8))));r[0]=0
 hooks={}
 for t in [0x8c10ee9c,0x8c10eee0,0x8c10ee20]:
  def hook(m,r,sr,t=t):service(t,m,r,expected,0);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c10d460:return transfer(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  try:service(t,m,r,actual,1)
  except Exception as exc:errors.append(repr(exc));return -9
  return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=address;s.r[5]=0x8cb02000;s.r[15]=0x8cf00000;s.pc=0x8c667150;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected and words[0]==words[1]
 assert r[0]==0 and r[15]==0x8cf00000
 allowed=address<0x200000 or address>=0x80000000
 if allowed:
  assert reads==[(0xa0800000+address)&0xffffffff]
  assert native[0xb01ffc:0xb02008]==bytes([0xa5])*4+value.to_bytes(4,'little')+bytes([0xa5])*4
 else:assert not actual and native[0xb01ffc:0xb02008]==bytes([0xa5])*12
 cases+=1
report={'cases_passed':cases,'instructions_recovered':22,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/event equality; seven offsets including all lanes, limit and negative signed values, three output words, context flag0/1, both FPU banks. Independent device alias, four-byte transfer, stride1 and zero stack arguments, output/tail guards, return0 and rejected address no-op. Native transfer10d460 connected; polling/control services10ee9c/10eee0/10ee20 modeled; actual hardware unverified.'}
(root/'analysis/sound_field_word_read_transfer_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'device word read comparisons passed')
