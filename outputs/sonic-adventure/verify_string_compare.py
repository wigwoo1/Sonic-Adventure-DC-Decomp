import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/string_compare.dll')).sa_string_compare
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 pairs=[(b'',b''),(b'abc',b'abc'),(b'abc',b'abd'),(b'abd',b'abc'),(b'abcdefghijk',b'abcdefghijz'),(b'ab',b'abc'),(b'abc',b'ab'),(b'HARD',b'Enable')]
 for (left,right),a,b,bank in itertools.product(pairs,range(4),range(4),[0,0x200000]):
  fill=0;mask=6
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  ram[0x8a2f80:0x8a3000]=bytes([fill])*128
  ram[0xa00000+a:0xa00000+a+len(left)+1]=left+b'\0'
  ram[0xa00100+b:0xa00100+b+len(right)+1]=right+b'\0'
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c10db1c;s.r[4]=0x8ca00000+a;s.r[5]=0x8ca00100+b
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  result=ctypes.c_int32(r[0]).value
  assert (result>0)-(result<0)==(left>right)-(left<right)
  cases+=1
 (out/'analysis/string_compare_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=81,scope='Complete retail RAM/CPU/FPU comparison of recovered optimized string comparison. Independent lexicographic sign checks for ASCII equal/unequal, prefix/empty and long strings across both pointer alignments and FPU banks; no injected calls. Covers CMP/STR through zero-byte detection. High-byte string semantics not independently validated.'),indent=2)+'\n')
 print(cases,'string comparison cases passed')
if __name__=='__main__':main()
