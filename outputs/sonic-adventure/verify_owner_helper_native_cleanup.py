"""Execute original state bodies with explicit nested-service fixtures.
Produces behavioral baseline; does not claim recovered source equivalence.
"""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b
 dll=ROOT/'work/sonic-adventure/build/owner_helper_native_cleanup.dll'
 calltype=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*5),ctypes.POINTER(ctypes.c_uint32))
 fn=ctypes.CDLL(str(dll)).sa_owner_helper_with_native_cleanup
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,calltype,ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int
 records=[]
 for selector,phase,step,next_state,mutation,previous,same_id,mask in itertools.product([2,8],[0.0,0.5,7.0,8.0,-1.0],[0.25,1.0],[3,6,12],["none","status"],[1],[False,True],range(4)):
  ram[0xa00000:0xa10000]=bytes(0x10000)
  struct.pack_into('<8H',ram,0xa00000,selector,0,1,previous,0,2,0,0)
  struct.pack_into('<2f',ram,0xa00010,phase,0.5)
  struct.pack_into('<2I',ram,0xa00018,0x8ca06000,0x8ca06000);struct.pack_into('<f',ram,0xa06000,1.0)
  struct.pack_into('<I',ram,0xa00020,0x8ca01000);struct.pack_into('<I',ram,0xa00028,0x8ca04000)
  for i in range(3):
   struct.pack_into('<I',ram,0xa01000+i*16,0x8ca02000)
   ram[0xa01004+i*16:0xa01008+i*16]=bytes([1,next_state,7,0])
   struct.pack_into('<2f',ram,0xa01008+i*16,step,0.5)
  struct.pack_into('<2I',ram,0xa02000,0x1234,0x8ca03000)
  struct.pack_into('<2I',ram,0xa03000,8,8);struct.pack_into('<2I',ram,0xa03100,8,16)
  struct.pack_into('<2I',ram,0xa04000,0x1234,0x8ca04100);struct.pack_into('<3I',ram,0xa04100,0x8ca05000,2,0x00020003)
  for i in [0,2]:struct.pack_into('<I',ram,0xa01000+i*16,0x8ca02200)
  struct.pack_into('<2I',ram,0xa02200,0x1234 if same_id else 0x5678,0x8ca03000)
  ram[0x880000:0x880100]=bytes(256)
  struct.pack_into('<I',ram,0x79f03c,0)
  for i in range(3):struct.pack_into('<4I',ram,0x880010+i*32,0,0,0x8c880010+(i+1)*32 if i<2 else 0,0x8c880010+(i-1)*32 if i else 0)
  struct.pack_into('<4I',ram,0xa05000,0x8c880020 if mask&1 else 0,0x8c880040 if mask&2 else 0,0xabcdef12,0xabcdef34)
  fpu=Fpu();fpu.fpscr=0x40001
  fpu.banks[0][14]=0x12345678;fpu.banks[0][15]=0x87654321
  state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=0x8ca00000;state.r[15]=0x8cf00000;state.pc=0x8c079476;state.pr=0x8c000100
  saved=list(state.r[8:]);events=[];allocation=[0x8ca07000]
  def service(address):
   def invoke(memory,regs,status):
    events.append(dict(address=hex(address),args=[hex(x) for x in regs[4:7]],incoming_r0=hex(regs[0])))
    if address==0x8c098a80:regs[0]=allocation[0];allocation[0]+=0x100
    elif address==0x8c098b0c:
     regs[0]=allocation[0];allocation[0]+=0x100
     start=regs[0]&0xffffff;memory[start:start+regs[5]]=bytes(regs[5])
    if mutation=="status":
     status^=0x01000031
     if address not in [0x8c098a80,0x8c098b0c]:regs[0]=0x12345678
    return status
   return invoke
  hooks={a:service(a) for a in [0x8c098a80,0x8c098b0c,0x8c079c20]}
  tail_hit=[]
  def tail(memory,regs,status):tail_hit.append(True);return status

  native=bytearray(ram)
  try:reference(ram,state,hooks,fpu)
  except Exception as error:raise AssertionError((selector,phase,str(error))) from error
  assert list(state.r[8:])==saved and state.pr==0x8c000100
  assert fpu.banks[0][14]==0x12345678 and fpu.banks[0][15]==0x87654321
  expected_events=list(events);events.clear();allocation[0]=0x8ca07000
  @calltype
  def native_service(ctx,address,argc,a,b,r0,status):
   regs=[0]*16;regs[0]=r0;regs[4]=a;regs[5]=b
   status[0]=service(address)(native,regs,status[0]);return regs[0]
  view=(ctypes.c_uint8*len(native)).from_buffer(native);status=ctypes.c_uint32(0)
  fpscr=ctypes.c_uint32(0x40001);fpul=ctypes.c_uint32(0);out_r0=ctypes.c_uint32()
  disposition=fn(view,ctypes.byref(status),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00000,None,native_service,ctypes.byref(out_r0))
  result=out_r0.value
  assert disposition==0 and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul
  assert native[0xa00000:0xa10000]==ram[0xa00000:0xa10000]
  assert native[0x880000:0x880100]==ram[0x880000:0x880100]
  assert native[0x79f03c:0x79f040]==ram[0x79f03c:0x79f040]
  assert result==state.r[0] and status.value==state.sr
  def meaningful(rows):return [(r['address'],r['args'][:2 if r['address']=='0x8c098b0c' else 1],r['incoming_r0']) for r in rows]
  assert meaningful(events)==meaningful(expected_events),(phase,step,previous,same_id,meaningful(events),meaningful(expected_events))
  records.append(dict(state=selector,initial_phase=phase,next_state=struct.unpack_from('<H',ram,0xa00000)[0],config_bytes=ram[0xa00000:0xa00030].hex(),r0=hex(state.r[0]),sr=hex(state.sr),fpscr=hex(fpu.fpscr),calls=events))
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=len(records),source_sha256=hashlib.sha256((out/'source/owner_helper_native_cleanup.c').read_bytes()).hexdigest(),dependency_sha256={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['owner_helper_dispatch.c','helper_record_release.c','heap_free_ram.c','owner_helper_prepare.c','owner_helper_state0.c','owner_helper_state1.c','owner_helper_state2.c','owner_helper_state2_tail.c','owner_helper_terminal.c','owner_helper_repeat.c','owner_helper_advance.c','owner_helper_clamp.c','owner_helper_state8.c','owner_helper_transition.c','owner_helper_tail.c','sh4_float_i32.c','sh4_add_sub.c','sh4_fmac.c']},dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(b).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='Integrated helper with native record cleanup and heap free compared with original instructions in phase/step, signed next-state and callback status fixtures, meaningful allocation arguments and incoming r0, initialized RAM, return and SR. Model transition hooked; record cleanup, heap free, shared tail and native arithmetic/conversion execute. States 2/8, record cleanup and heap free execute without hooks; model transition remains hooked.',cases=records)
 (out/'analysis/owner_helper_native_cleanup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(records)} original helper state fixtures executed')
if __name__=='__main__':main()

