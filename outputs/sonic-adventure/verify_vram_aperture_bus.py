"""Exhaustive address permutation plus native bus, alias and failure controls.

Authored platform tests give zero original-game recovery credit.
"""
import ctypes,hashlib,json,random,struct
from pathlib import Path
import vram_aperture_reference as model
P=Path(__file__).resolve().parent;ROOT=P.parents[1];U=ctypes.c_uint32
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,ctypes.c_uint,ctypes.POINTER(U))
Write=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,ctypes.c_uint)
Cache=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,ctypes.c_uint)
class Bus(ctypes.Structure):_fields_=[('context',ctypes.c_void_p),('read',Read),('write',Write),('cache',Cache)]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 dll=ROOT/'work/sonic-adventure/build/movie_buffer_initialize.dll';lib=ctypes.CDLL(str(dll));State=model.state_type(Bus);model.declare(lib,Bus,State)
 seen=bytearray(0x200000)
 for offset in range(0,0x800000,4):
  mapped=model.mapping(0xa4000000+offset)-0xa5000000;assert not mapped&3 and 0<=mapped<0x800000 and not seen[mapped//4];seen[mapped//4]=1
  # Reconstruct the interleaved physical word by bank and position.
  bank=mapped//0x400000;position=(mapped%0x400000)//4;assert offset==(position*2+bank)*4
 assert all(seen)
 memory={};calls=[];fail=[0]
 @Read
 def read(ctx,a,width,out):
  calls.append(('read',a,width))
  if fail[0]:return fail[0]
  out[0]=memory.get(a,0x12345678);return 0
 @Write
 def write(ctx,a,value,width):
  calls.append(('write',a,value,width))
  if fail[0]:return fail[0]
  memory[a]=value;return 0
 @Cache
 def cache(ctx,a,op):calls.append(('cache',a,op));return fail[0]
 parent=Bus(None,read,write,cache);bus=Bus();state=State();trace=(model.Event*64)();reference=model.Reference(64)
 assert lib.sa_vram_aperture_bus_init(ctypes.byref(state),ctypes.byref(parent),ctypes.byref(bus),trace,len(trace))==0
 offsets=[0,4,8,12,0x3ffffc,0x400000,0x400004,0x7ffff8,0x7ffffc];rng=random.Random(0x65393a);offsets += [rng.randrange(0x200000)*4 for _ in range(256)]
 native_calls=0
 for alias in (0x04000000,0x84000000,0xa4000000):
  for index,offset in enumerate(offsets):
   a=alias+offset;expected=model.mapping(a);value=(a^0x13579bdf^index)&0xffffffff
   assert bus.write(bus.context,a,value,4)==0 and calls[-1]==('write',expected,value,4);reference.attempts+=1;reference.record(a,expected,value,2);native_calls+=1
   out=U(0);assert bus.read(bus.context,a,4,ctypes.byref(out))==0 and out.value==value and calls[-1]==('read',expected,4);reference.attempts+=1;reference.record(a,expected,value,1);native_calls+=1
 for address,width in ((0xa4000001,4),(0xa4000002,4),(0xa4000000,1),(0xa4000000,2),(0xa4000000,8)):
  before=len(calls);out=U(0xa5a5a5a5);assert bus.read(bus.context,address,width,ctypes.byref(out))==-3 and bus.write(bus.context,address,7,width)==-3 and out.value==0xa5a5a5a5 and len(calls)==before;reference.attempts+=2;native_calls+=2
 fail[0]=-7;out=U(0xa5a5a5a5)
 assert bus.read(bus.context,0xa4000020,4,ctypes.byref(out))==-7 and out.value==0xa5a5a5a5
 assert bus.write(bus.context,0xa4000020,12,4)==-7;reference.attempts+=2;native_calls+=2
 # Outside the supported window and cache operations retain the parent's status.
 attempts=state.attempts
 for address in (0xa3fffffc,0xa4800000,0xa5000000,0x8c010000):
  assert bus.read(bus.context,address,4,ctypes.byref(out))==-7 and calls[-1]==('read',address,4)
  assert bus.write(bus.context,address,23,4)==-7 and calls[-1]==('write',address,23,4);native_calls+=2
 assert bus.cache(bus.context,0x8c123400,2)==-7 and calls[-1]==('cache',0x8c123400,2) and state.attempts==attempts;native_calls+=1;fail[0]=0
 assert model.snapshot(state)==reference.snapshot()
 assert ctypes.string_at(state.trace,state.count*16)==b''.join(struct.pack('<IIII',*e) for e in reference.events[:64])
 # Zero-capacity observation still performs real parent writes and tracks drops.
 empty=State();empty_bus=Bus();assert lib.sa_vram_aperture_bus_init(ctypes.byref(empty),ctypes.byref(parent),ctypes.byref(empty_bus),None,0)==0
 assert empty_bus.write(empty_bus.context,0xa4000004,0xabcdef01,4)==0 and memory[0xa5400000]==0xabcdef01 and (empty.count,empty.dropped,empty.attempts)==(0,1,1);native_calls+=1
 assert lib.sa_vram_aperture_bus_init(ctypes.byref(empty),ctypes.byref(parent),ctypes.byref(empty_bus),None,1)==-3
 files=[Path(__file__),Path(model.__file__),P/'source/vram_aperture_bus.c',P/'source/vram_aperture_bus.h',dll]
 evidence=[];commit='e88b0ced2243de137bcccac0072f0c1dd00d41bd'
 for source in ('core/hw/pvr/pvr_mem.cpp','core/hw/sh4/modules/dmac.cpp','core/hw/pvr/pvr_sb_regs.cpp'):
  path=ROOT/('work/sonic-adventure/aperture-primary-'+source.split('/')[-1]);files.append(path);evidence.append(dict(url='https://github.com/flyinghead/flycast/blob/'+commit+'/'+source,local_normalized_sha256=digest(path)))
 report=dict(platform_backend_only=True,original_recovery_credit=0,exhaustive_aligned_word_addresses=0x200000,address_permutation_bijective=True,native_bus_calls=native_calls,aliases_checked=['0x04000000','0x84000000','0xa4000000'],parent_layout='Existing bank-linear 0x05000000 window; no earlier source or storage layout changes',parent_failure_status_preserved=True,unsupported_width_and_alignment_rejected=True,trace_capacity_zero_and_overflow_checked=True,all_retained_trace_bytes_match=True,primary_sources=evidence,dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files},scope='Independent exhaustive 8MiB alignedword permutation;native read/write alias,boundary,misalignment,width,parentfailure,pass-through,cache andbounded-observation controls. Authored platform backend,notoriginalroutinecredit;notphysicaltiming,byte/halfwordVRAMorothermirrors. No emulator implementation copied or invoked.')
 (P/'analysis/vram_aperture_bus_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Aperture permutation words',0x200000,'native calls',native_calls,'all controls passed')

if __name__=='__main__':main()
