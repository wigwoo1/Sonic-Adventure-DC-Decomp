from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_engine_transfer_wait.py').read_text().replace('from verify_tikal_controller import Call,P','from verify_tikal_controller import Call,P\nfrom verify_engine_cache_writeback import IO,Read,Write')
s=s.replace('cases=0', "ready=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_ready_get.dll')).sa_engine_transfer_ready_get;ready.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]\ncases=0")
s=s.replace('[(1,),(0,1),(2,0,1),(0xffffffff,2,0,1)]','[(0,),(32,0),(0xffffffff,33,31),(0x80000020,63,32,0x80000000)]')
a=s.index(' @Call');z=s.index(' s=State();',a)
s=s[:a]+""" reads=[];errors=[]
 @Read
 def read(ctx,a,n):
  assert a==0xa05f688c and n==4 and len(reads)<len(sequence)
  value=sequence[len(reads)];reads.append((a,n));return value
 @Write
 def write(ctx,a,v,n):errors.append((a,v,n))
 io=IO(None,read,write)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10fb48
  actual.append(r[4]);return ready(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
 class Registers(dict):
  def __init__(self):super().__init__({0xa05f688c:0});self.reads=[]
  def __getitem__(self,a):
   assert a==0xa05f688c and len(self.reads)<len(sequence)
   value=sequence[len(self.reads)];self.reads.append(a);return value
 mmio=Registers()
"""+s[z:]
s=s.replace('reference(ram,s,{0x8c10fb48:status},fpu=f)','reference(ram,s,{},fpu=f,mmio=mmio)')
s=s.replace('assert actual==expected==[0x8ceffff0]*len(sequence)', 'assert actual==[0x8ceffff0]*len(sequence) and reads==[(0xa05f688c,4)]*len(sequence) and mmio.reads==[0xa05f688c]*len(sequence) and not errors')
a=s.index("scope='");z=s.index("\n(p/'analysis/",a)
s=s[:a]+"scope='32 full retail RAM/CPU/FPU/SR/PR comparisons of native transfer wait10eee0 connected with native readiness getter10fb48. Four controlled MMIO register sequences,immediate and delayed readiness,unrelated high bits,four counters including overflow,both banks. Independent exact read order/count at a05f688c,width4,stack status pointer,inverted hardware bit5,one context counter increment+10c,neighbor guards,status0 and stack restoration. No modeled calls. Nonterminating devices and physical hardware/full game integration unverified.'"+s[z:]
s=s.replace('engine_transfer_wait_verification.json','engine_transfer_wait_native_ready_verification.json')
(p/'verify_engine_transfer_wait_native_ready.py').write_text(s)
reg=(p/'register_backend_readback_native_descriptor.py').read_text().replace("['backend_readback']","['engine_transfer_wait']").replace('backend_readback_native_descriptor_verification.json','engine_transfer_wait_native_ready_verification.json').replace('==28','==32').replace("{'sa_sound_backend_readback','sa_sound_descriptor_byte_query'}","{'sa_engine_transfer_wait','sa_engine_transfer_ready_get'}").replace('native_descriptor','native_ready').replace('native diagnostics:', 'native readiness:')
(p/'register_engine_transfer_wait_native_ready.py').write_text(reg)
