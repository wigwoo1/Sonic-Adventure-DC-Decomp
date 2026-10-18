import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_change_initialize.dll';fn=ctypes.CDLL(str(dll)).sa_owner_model_change_initialize;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32)];cases=0
 for count,cf,pf,nodes in itertools.product([0,1,8,255],[0,4],[0,4],[1,3,8]):
  for m in [old,native]:
   m[0xa00000:0xa08000]=bytes(0x8000);m[0xf00000:0xf00400]=bytes([0xa5])*1024
   struct.pack_into('<2H',m,0xa00004,1,2);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,0xa00028,0x8ca02000)
   struct.pack_into('<I',m,0xa01010,0x8ca03000);m[0xa01014]=count;struct.pack_into('<I',m,0xa01020,0x8ca03100)
   struct.pack_into('<2I',m,0xa03000,0x8ca06000,0x8ca04000);struct.pack_into('<I',m,0xa03104,0x8ca04100)
   struct.pack_into('<I',m,0xa04000,0x8ca07000);struct.pack_into('<H',m,0xa04008,cf);struct.pack_into('<I',m,0xa04100,0x8ca07100);struct.pack_into('<H',m,0xa04108,pf)
   struct.pack_into('<I',m,0xa02004,0x8ca02100);struct.pack_into('<I',m,0xa02100,0x8ca05000)
   for i in range(nodes):struct.pack_into('<2I',m,0xa0602c+i*64,0x8ca06000+(i+1)*64 if i+1<nodes else 0,0)
  state=State();state.r[4]=0x8ca00000;state.r[15]=0x8cf00000;state.pc=0x8c079c90;state.pr=0x8c000100;hit=[]
  def stop(address):
   def invoke(memory,regs,sr):hit.append(address);state.pr=0x8c000100;return sr
   return invoke
  reference(old,state,{a:stop(a) for a in [0x8c079d26,0x8c07a456]})
  sr=ctypes.c_uint32(0);out=(ctypes.c_uint32*6)();fn(view,ctypes.byref(sr),0x8ca00000,0x8cf00000,out)
  assert list(out)==[state.r[8],state.r[9],state.r[10],state.r[11],state.r[0],hit[0]] and sr.value==state.sr
  assert native[0xf00000:0xf00400]==old[0xf00000:0xf00400] and native[0xa00000:0xa08000]==old[0xa00000:0xa08000];cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/owner_model_change_initialize.c').read_bytes()).hexdigest(),scope='Internal worker initialization with native recursive descriptor collection. Count 0/1/8/255, mode flags and hierarchy depths. Working-frame RAM, holder ID, cached pointers, r0/SR and next branch. Record loop and finalization are stop hooks; full worker incomplete.')
 (outdir/'analysis/owner_model_change_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} native initialization comparisons passed')
if __name__=='__main__':main()
