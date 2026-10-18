import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/owner_model_change_advance.dll')).sa_owner_model_change_advance;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*5+[ctypes.POINTER(ctypes.c_uint32)];cases=0
 for cm,pm,count,index,sr0 in itertools.product([2,3],[2,3],[0,1,8,255],[0,1,7,254,255,0xffffffff],[0,0x700083f3]):
  for mem in [old,native]:
   mem[0xa00000:0xa02000]=bytes(8192);mem[0xf00000:0xf00100]=bytes([0xa5])*256
   struct.pack_into('<I',mem,0xa00020,0x8ca01000);mem[0xa01014]=count
   struct.pack_into('<2I',mem,0xf00014,pm,cm);struct.pack_into('<2I',mem,0xf00028,index,16)
  state=State();state.r[12]=0x8ca00000;state.r[15]=0x8cf00000;state.r[9]=0x8ca00100;state.r[10]=0x8ca00200;state.r[11]=0x8ca00300;state.sr=sr0;state.pc=0x8c07a414;state.pr=0x8c000100;hit=[]
  def stop(address):
   def invoke(memory,regs,sr):hit.append(address);state.pr=0x8c000100;return sr
   return invoke
  reference(old,state,{a:stop(a) for a in [0x8c079d26,0x8c07a456]})
  sr=ctypes.c_uint32(sr0);out=(ctypes.c_uint32*5)();fn(view,ctypes.byref(sr),0x8ca00000,0x8cf00000,0x8ca00100,0x8ca00200,0x8ca00300,out)
  assert list(out)==[state.r[9],state.r[10],state.r[11],state.r[0],hit[0]] and sr.value==state.sr
  assert native[0xf00000:0xf00100]==old[0xf00000:0xf00100];cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/owner_model_change_advance.c').read_bytes()).hexdigest(),scope='Original model worker record loop advance. Independent mode strides, unsigned index wrap, count boundaries, SR and next branch. Integration with channels pending.')
 (outdir/'analysis/owner_model_change_advance_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} loop advance comparisons passed')
if __name__=='__main__':main()
