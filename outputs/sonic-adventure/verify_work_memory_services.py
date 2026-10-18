import ctypes,hashlib,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/work_memory_services.dll'));counts={}
 for name,entry in [('move',0x8c10d25c),('fill',0x8c10d31c)]:
  fn=getattr(lib,'sa_memory_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_void_p,Call];count=0
  for alignment,delta,length in itertools.product(range(4),[-260,-4,-1,0,1,4,260],[0,1,2,3,4,8,15,152,256]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa00000:0xa01000]=bytes((i*17+3)&255 for i in range(4096))
   dest=0x8ca00800+alignment;source=dest+delta;value=(delta*31)&0xffffffff;original=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
   f=Fpu();nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=dest;s.r[5]=source if name=='move' else value;s.r[6]=length;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();errors=[]
   @Call
   def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
   reference(ram,s,fpu=f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   wanted=bytearray(original);a=dest&0xffffff;b=source&0xffffff;wanted[a:a+length]=original[b:b+length] if name=='move' else bytes([value&255])*length
   assert native[0xa00000:0xa01000]==wanted[0xa00000:0xa01000] and r[0]==dest
   count+=1
  counts[name]=count
 report=dict(cases_passed=counts,instruction_counts=dict(move=95,fill=12),binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/('sa_memory_'+n+'.c')).read_bytes()).hexdigest() for n in counts},scope='Full retail state comparisons; independent byte results for zero and small/context sizes, all destination alignments, forward/backward overlap and identical pointers. No injected services. Very large halfword counts and unmapped/MMIO addresses untested.')
 (out/'analysis/work_memory_services_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'memory service comparisons passed')
if __name__=='__main__':main()
