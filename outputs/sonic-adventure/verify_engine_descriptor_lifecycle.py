import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_descriptor_lifecycle.dll'))
allocate=lib.sa_engine_descriptor_allocate;allocate.argtypes=abi;release=lib.sa_engine_descriptor_release;release.argtypes=abi
cases=0
for free_index,flag,bank in itertools.product([0,1,4095,None],[0,0xfffe],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0x8a83e4:0x8c03e4]=bytes([0xa5])*0x18000
 if free_index is not None:struct.pack_into('<H',ram,0x8a83e4+free_index*24,flag)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 @Call
 def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 def run(pc,fn,argument=None):
  s.pc=pc;s.pr=0x8c000100
  if argument is not None:s.r[4]=argument
  r[:]=s.r;sr.value=s.sr;pr.value=s.pr
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  return r[0]
 pointer=run(0x8c664ac0,allocate)
 assert pointer==(0 if free_index is None else 0x8c8a83e4+free_index*24)
 if pointer:
  offset=pointer&0xffffff;assert native[offset:offset+24]==b'\x01\x00'+bytes([0xa5])*22
  run(0x8c664aec,release,pointer)
  assert native[offset:offset+24]==bytes(2)+bytes([0xa5])*22
  assert run(0x8c664ac0,allocate)==pointer
 else:assert native[0x8a83e4:0x8c03e4]==bytes([0xa5])*0x18000
 cases+=1
report={'cases_passed':cases,'instructions_recovered':41,'scope':'Full retail RAM/CPU/FPU/SR/PR comparison of22-instruction allocator and19-instruction release, first/second/final free entry and exhaustion, two free-flag patterns and FPU banks. Independent selected pointer, allocated flag, release clearing and reallocation,22-byte tail preservation. All4096 pool entries scanned for exhaustion; no external services modeled.'}
(root/'analysis/engine_descriptor_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'native descriptor lifecycle comparisons passed')
