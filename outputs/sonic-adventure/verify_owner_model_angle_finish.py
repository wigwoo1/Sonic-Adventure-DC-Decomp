import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_angle_finish.dll';fn=ctypes.CDLL(str(dll)).sa_owner_model_angle_finish;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*3;fn.restype=ctypes.c_uint32
 rng=random.Random(0xa0fe);values=[0,1,0x7fff,0x8000,0xffff,0x12348000,0xffffffff];pairs=list(itertools.product(values,repeat=2))+[(rng.getrandbits(32),rng.getrandbits(32)) for _ in range(1000)];cases=0
 for a,b in pairs:
  for m in [old,native]:
   m[0xa00000:0xa00300]=bytes([0xa5])*768;struct.pack_into('<I',m,0xf00000,0x8ca00100)
   for c in [4,8,12]:struct.pack_into('<I',m,0xa00100+c,(a+c)&0xffffffff);struct.pack_into('<I',m,0xa00110+c,(b-c)&0xffffffff)
  state=State();state.r[15]=0x8cf00000;state.r[11]=0x8ca00200;state.r[8]=2;state.pc=0x8c07a0fe;state.pr=0x8c000100;state.sr=0x700083f3
  def stop(memory,r,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c07a1c4:stop});sr=ctypes.c_uint32(0x700083f3);result=fn(view,ctypes.byref(sr),0x8cf00000,0x8ca00200,2)
  assert result==state.r[0] and sr.value==state.sr
  assert old[0xa00000:0xa00300]==native[0xa00000:0xa00300] and old[0xeffffc:0xf00004]==native[0xeffffc:0xf00004];cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/owner_model_angle_finish.c').read_bytes()).hexdigest(),scope='Angle-channel finalization: signed-half normalization, three native unwrap calls, slot mode write, scratch pointer, RAM/r0/SR. Boundary/random full-width vectors; next-channel entry hooked, unwrap calls unhooked. Whole channel integration pending.')
 (out/'analysis/owner_model_angle_finish_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} angle finalization comparisons passed')
if __name__=='__main__':main()
