import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0x638f94)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_bank_save.dll')).sa_model_matrix_bank_save_8c638f94;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.c_uint32];cases=0
 for bank,rm,sr0,pattern in itertools.product(range(2),range(4),[0,0x700083f3],range(64)):
  fpu=Fpu();fpu.fpscr=0x7f07c|(bank<<21)|rm
  for k in range(2):fpu.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  fpu.fpul=0xabcdef12;expected=Fpu.from_buffer_copy(fpu);state=State();state.r[4]=0x8ca00000;state.pc=0x8c638f94;state.pr=0x8c000100;state.sr=sr0;initial=list(state.r);reference(old,state,{},expected);result=fn(view,ctypes.byref(fpu),0x8ca00000)
  assert result==0 and old[0xa00000:0xa00040]==native[0xa00000:0xa00040] and bytes(fpu)==bytes(expected) and list(state.r)==initial and state.sr==sr0;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_bank_save_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_matrix_bank_save.c').read_bytes()).hexdigest(),scope='Original matrix-bank save through return without hooks. Both FR banks, RM bit patterns, random raw registers and SR; RAM, all integer registers and FPU state compared. PR0/SZ0 only; hardware unverified.'),indent=2)+'\n');print(f'{cases} matrix bank save comparisons passed')
if __name__=='__main__':main()
