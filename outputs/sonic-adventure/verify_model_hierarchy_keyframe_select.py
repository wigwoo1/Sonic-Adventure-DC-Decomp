import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_keyframe_select.dll')).sa_model_hierarchy_keyframe_select;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),P,P,ctypes.c_uint32,ctypes.c_uint,P];cases=0
 from verify_fpu_cleanup_transfer import Fpu
 for mode,sr0,keys,frame,channel,total in itertools.product([2,3],[0,0x700083f3],[(0,2,7,12,100),(0,1,2,3,100),(0x80000000,0x80000002,0xfffffffe,0xffffffff,0xffffffff)], [0,1,2,3,6,7,11,12,13,0x80000000,0x80000001,0xfffffffe],range(3),[4,13,0xffffffff]):
  # Increasing fixtures keep original lookahead mapped and prevent walking beyond final sentinel.
  if frame>keys[-2]:continue
  for i,key in enumerate(keys):struct.pack_into('<I',ram,0xa02000+i*16,key)
  struct.pack_into('<I',ram,0xa01000+channel*4,0x8ca02000);struct.pack_into('<I',ram,0xa0010c,total);state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[8]=0x8ca01000;state.r[7]=channel*4;state.r[14]=0x8ca00100;state.pc=0x8c057cc4 if mode==2 else 0x8c057ee8;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r);f=Fpu();f.fpul=frame;dest=[]
  def stop(pc):
   def apply(m,regs,s):dest.append(pc);state.pr=0x8c000100;return s
   return apply
  stops=[0x8c057cf6,0x8c057d16] if mode==2 else [0x8c057f1a,0x8c057f3a];before=bytes(ram);reference(ram,state,{pc:stop(pc) for pc in stops},f);sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(sr),r,frame,mode,ctypes.byref(nextpc))==0
  assert list(r)==list(state.r) and sr.value==state.sr and nextpc.value==dest[0] and bytes(ram)==before,(keys,frame);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_keyframe_select_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_keyframe_select.c').read_bytes()).hexdigest(),scope='Both layout keyframe selection loops through factor branch, no function hooks beyond stop. Full integer/SR/branch/RAM compared; unsigned high keys and boundary frames, channel offsets, wrap-terminal/nonterminal cases. Frame conversion and interpolation remain pending.'),indent=2)+'\n');print(cases,'keyframe selection comparisons passed')
if __name__=='__main__':main()
