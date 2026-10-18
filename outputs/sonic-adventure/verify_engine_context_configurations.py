import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 for fill,bank,mask,config in itertools.product([0,0x55,0xff],[0,0x200000],[0,15],[b'REND',b'HARD',b'Enable']):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  ram[0x8a2d54:0x8a3224]=bytes([fill])*0x4d0
  ram[0x671a4c:0x671a54]=(config+b'\0').ljust(8,b'\0')
  ram[0x671a98:0x671aa8]=(config+b'\0').ljust(16,b'\0')
  struct.pack_into('<I',ram,0x67c004,0 if fill==0 else 0x8c8a2d54)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c6510a4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  defaults=[0x8c641698,0x8c64169c,0x8c6416a0,0x8c6416a4,0x8c6416e0,0x8c6416e4,0x8c6416e8,0x8c6416ec,0x8c6416f0]
  for i,entry in enumerate(defaults):assert struct.unpack_from('<2I',native,0x8a2f84+i*8)==(entry,0)
  assert struct.unpack_from('<I',native,0x8a2ff8)[0]==0
  assert struct.unpack_from('<I',native,0x67c004)[0]==0x8c8a2d54
  assert struct.unpack_from('<I',native,0x8a2f00)[0]==0x01000000
  cases+=1
 (out/'analysis/engine_context_configurations_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=264,scope='Full engine context initializer with native memory fill, default handlers, parameter setup, configuration selector and string comparison; no injected services. Complete retail RAM/CPU/FPU comparisons. Independent fixed context pointer, nine callback/payload pairs and parameter flags checked with initially unset/existing pointers, context patterns and both FPU banks. Runs REND, HARD and Enable configuration source fixtures for both configuration groups; full retail state comparison verifies alternate selection branches. Alternate configuration values are test mutations, not inferred user-facing settings. Does not implement host exception delivery or game boot.'),indent=2)+'\n')
 print(cases,'interrupt defaults comparisons passed')
if __name__=='__main__':main()
