"""Execute original state bodies with explicit nested-service fixtures.
Produces behavioral baseline; does not claim recovered source equivalence.
"""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b
 dll=ROOT/'work/sonic-adventure/build/owner_helper_terminal.dll'
 calltype=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*5),ctypes.POINTER(ctypes.c_uint32))
 fn=ctypes.CDLL(str(dll)).sa_owner_helper_terminal
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32,ctypes.c_void_p,calltype];fn.restype=ctypes.c_uint32
 records=[]
 for selector,phase,mutation,payload_present in itertools.product([13,14,15,65535],[0.0,0.5,1.0,8.0,-1.0],["none","drop","retarget","free_retarget","status"],[False,True]):
  ram[0xa00000:0xa10000]=bytes(0x10000)
  struct.pack_into('<8H',ram,0xa00000,selector,0,1,1,0,0,0,0)
  struct.pack_into('<2f',ram,0xa00010,phase,0.5)
  struct.pack_into('<2I',ram,0xa00018,0x8ca06000,0x8ca06000);struct.pack_into('<f',ram,0xa06000,1.0)
  struct.pack_into('<I',ram,0xa00020,0x8ca01000);struct.pack_into('<I',ram,0xa00028,0x8ca04000)
  for i in range(3):
   struct.pack_into('<I',ram,0xa01000+i*16,0x8ca02000)
   ram[0xa01004+i*16:0xa01008+i*16]=bytes([1,3,7,0])
   struct.pack_into('<2f',ram,0xa01008+i*16,0.25,0.5)
  struct.pack_into('<2I',ram,0xa02000,0x1234,0x8ca03000)
  struct.pack_into('<2I',ram,0xa03000,8,0x8ca03100);struct.pack_into('<2I',ram,0xa03100,8,16)
  struct.pack_into('<2I',ram,0xa04000,0x1234,0x8ca04100);struct.pack_into('<3I',ram,0xa04100,0x8ca05000,2,0x00020003)
  struct.pack_into('<I',ram,0xa04100,0x8ca05000 if payload_present else 0)
  struct.pack_into('<2I',ram,0xa04200,0x1234,0x8ca04300);struct.pack_into('<I',ram,0xa04300,0x8ca04400)
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
    if address==0x8c07a49c and mutation in ["drop","retarget"]:struct.pack_into('<I',memory,0xa00028,0 if mutation=="drop" else 0x8ca04200)
    if address==0x8c098b42 and mutation=="free_retarget" and len(events)==2:struct.pack_into('<I',memory,0xa00028,0x8ca04200)
    if mutation=="status":status^=0x01000031;regs[0]=0x12345678^address
    return status
   return invoke
  hooks={a:service(a) for a in [0x8c098a80,0x8c098b0c,0x8c098b42,0x8c079c20,0x8c07a49c]}
  native=bytearray(ram)
  try:reference(ram,state,hooks,fpu)
  except Exception as error:raise AssertionError((selector,phase,str(error))) from error
  assert list(state.r[8:])==saved and state.pr==0x8c000100
  assert fpu.banks[0][14]==0x12345678 and fpu.banks[0][15]==0x87654321
  expected_events=list(events);events.clear();allocation[0]=0x8ca07000
  # State selection increments the timer before entering this body.
  struct.pack_into('<H',native,0xa0000c,1);struct.pack_into('<H',native,0xa00008,1)
  @calltype
  def native_service(ctx,address,argc,a,b,r0,status):
   regs=[0]*16;regs[0]=r0;regs[4]=a;regs[5]=b
   status[0]=service(address)(native,regs,status[0]);return regs[0]
  view=(ctypes.c_uint8*len(native)).from_buffer(native);status=ctypes.c_uint32(0)
  result=fn(view,ctypes.byref(status),0x8ca00000,1526 if selector<15 else (selector if selector<32768 else selector|0xffff0000),None,native_service)
  assert native[0xa00000:0xa10000]==ram[0xa00000:0xa10000]
  assert result==state.r[0] and status.value==state.sr
  def meaningful(rows):return [(r['address'],r['args'][:2 if r['address']=='0x8c098b0c' else 1],r['incoming_r0']) for r in rows]
  assert meaningful(events)==meaningful(expected_events)
  records.append(dict(state=selector,initial_phase=phase,next_state=struct.unpack_from('<H',ram,0xa00000)[0],config_bytes=ram[0xa00000:0xa00030].hex(),r0=hex(state.r[0]),sr=hex(state.sr),fpscr=hex(fpu.fpscr),calls=events))
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=len(records),source_sha256=hashlib.sha256((out/'source/owner_helper_terminal.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(b).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='Structured C terminal state body compared with original instructions in phase, payload presence and callback mutation fixtures, meaningful allocation arguments and incoming r0, cleanup RAM, return and SR. Nested record cleanup and heap frees hooked. Only terminal state body, not complete routine; other state bodies and native dependency integration pending.',cases=records)
 (out/'analysis/owner_helper_terminal_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(records)} original helper state fixtures executed')
if __name__=='__main__':main()

