import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 dll=ROOT/'work/sonic-adventure/build/model_angle_unwrap.dll';fn=ctypes.CDLL(str(dll)).sa_model_angle_unwrap_8c055d70;fn.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_uint32
 rng=random.Random(0x55d4e);pairs=list(itertools.product([0,1,0x7fff,0x8000,0xffff,0xffffffff,0x10000],repeat=2))+[(rng.randrange(-1048576,1048576)&0xffffffff,rng.randrange(-1048576,1048576)&0xffffffff) for _ in range(4096)]
 for a,b in pairs:
  state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=a;state.r[5]=b;state.pc=0x8c055d70;state.pr=0x8c000100;state.sr=rng.getrandbits(32);initial_sr=state.sr
  regs=(ctypes.c_uint32*16)(*state.r);reference(ram,state);status=ctypes.c_uint32(initial_sr);result=fn(regs,ctypes.byref(status))
  assert result==state.r[0] and list(regs)==list(state.r) and state.sr==status.value
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=len(pairs),routine_start='0x8c055d70',routine_end_exclusive='0x8c055dac',source_sha256=hashlib.sha256((out/'source/model_angle_unwrap.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(binary[0x45d70:0x45dac]).hexdigest(),scope='Complete original integer body, all integer registers, SR, boundary and deterministic random operands; no hooks.')
 (out/'analysis/model_angle_unwrap_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(pairs)} model angle unwrap comparisons passed')
if __name__=='__main__':main()
