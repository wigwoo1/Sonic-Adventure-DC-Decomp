"""Structured C selection section versus original execution up to state body."""
import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
class Selection(ctypes.Structure):_fields_=[('state',ctypes.c_uint32),('descriptor_offset',ctypes.c_uint32),('target',ctypes.c_uint32)]
def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_helper_prepare.dll';fn=ctypes.CDLL(str(dll)).sa_owner_helper_prepare
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(Selection)];fn.restype=None
 targets={0x8c07950e+struct.unpack_from('<h',binary,0x69510+i*2)[0] for i in range(15)}|{0x8c079b04}
 rng=random.Random(0x79476);states=list(range(16))+[32767,32768,65535]+[rng.randrange(65536) for _ in range(100)]
 cases=0
 for initial,current,previous,tick,sr0 in itertools.product(states,[0,1,65535],[0,1,65535],[0,32767,65535],[0,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa00030]=bytes([0xa5])*48
   struct.pack_into('<H',memory,0xa00000,initial);struct.pack_into('<2H',memory,0xa00004,current,previous);struct.pack_into('<H',memory,0xa0000c,tick)
  state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=0x8ca00000;state.r[15]=0x8cf00000;state.pc=0x8c079476;state.pr=0x8c000100;state.sr=sr0
  fpu=Fpu();fpu.fpscr=0x40001;hit=[]
  def hook(a):
   def invoke(memory,regs,status):hit.append((a,regs[13]));return status
   return invoke
  reference(old,state,{a:hook(a) for a in targets},fpu)
  sr=ctypes.c_uint32(sr0);selection=Selection();fn(view,ctypes.byref(sr),0x8ca00000,ctypes.byref(selection))
  raw=struct.unpack_from('<H',old,0xa00000)[0];signed_state=raw if raw<32768 else raw|0xffff0000
  assert old[0xa00000:0xa00030]==native[0xa00000:0xa00030]
  assert hit==[(selection.target,selection.descriptor_offset)] and selection.state==signed_state
  assert sr.value==state.sr,(initial,current,previous)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/owner_helper_prepare.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),retail_prefix_sha256=hashlib.sha256(binary[0x69476:0x6952e]).hexdigest(),scope='Structured C for helper state-selection front section: cached indices, timer wrapping, state restart exclusions, signed state/default target, descriptor offset, memory and SR. Original state bodies intercepted. This is a verified partial translation, not a complete recovered routine; floating-point locals/stack ABI remain in full parent work.')
 (out/'analysis/owner_helper_prepare_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} helper state-selection comparisons passed')
if __name__=='__main__':main()
