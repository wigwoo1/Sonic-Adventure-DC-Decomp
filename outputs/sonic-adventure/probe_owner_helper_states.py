"""Execute original state bodies with explicit nested-service fixtures.
Produces behavioral baseline; does not claim recovered source equivalence.
"""
import hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b
 records=[]
 for selector,phase in itertools.product(list(range(16))+[65535],[0.0,0.5,1.0,8.0,-1.0]):
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
    return status
   return invoke
  hooks={a:service(a) for a in [0x8c098a80,0x8c098b0c,0x8c098b42,0x8c079c20,0x8c07a49c]}
  try:reference(ram,state,hooks,fpu)
  except Exception as error:raise AssertionError((selector,phase,str(error))) from error
  assert list(state.r[8:])==saved and state.pr==0x8c000100
  assert fpu.banks[0][14]==0x12345678 and fpu.banks[0][15]==0x87654321
  records.append(dict(state=selector,initial_phase=phase,next_state=struct.unpack_from('<H',ram,0xa00000)[0],config_bytes=ram[0xa00000:0xa00030].hex(),r0=hex(state.r[0]),sr=hex(state.sr),fpscr=hex(fpu.fpscr),calls=events))
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_executed=len(records),binary_sha256=hashlib.sha256(b).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='Original owner helper execution baseline with all 15 table states plus two default selectors, five phase fixtures, nested allocation/initialization/cleanup hooks. Callee-saved registers and FPU saves checked. Parent source not translated; this is neither hardware validation nor C equivalence.',cases=records)
 (out/'analysis/owner_helper_original_state_baseline.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(records)} original helper state fixtures executed')
if __name__=='__main__':main()
