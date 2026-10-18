import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_dispatch.dll';lib=ctypes.CDLL(str(dll));fn=lib.sa_interrupt_dispatch_primary
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for dispatcher,depth,mutation,bank,mask in itertools.product(['primary','secondary'],range(5),range(3),[0,0x200000],[0,15]):
  fn=getattr(lib,'sa_interrupt_dispatch_'+dispatcher);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;base=0x8ca00000
  struct.pack_into('<I',ram,(0x893a80 if dispatcher=='primary' else 0x893a88),base if depth else 0)
  for i in range(depth):struct.pack_into('<4I',ram,0xa00000+i*16,0x8c010000+i*2,base+(i+1)*16 if i+1<depth else 0,0x12340000+i,0x80000000+i)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,target,pr):
   i=(target-0x8c010000)//2;trace=(target,r[4],sr,pr,r[15])
   if not i and mutation:struct.pack_into('<I',mem,0xa00004,base+32 if mutation==2 and depth>=3 else 0)
   for j in range(4):r[j]=0x34560000+j
   return trace,sr^0x31
  @Call
  def call(ctx,target,m,f,sr,r,pr):
   if target not in [0x8c010000+i*2 for i in range(depth)]:errors.append(hex(target));return -9
   trace,sr[0]=effect(native,r,sr[0],target,pr[0]);actual.append(trace);return 0
  def hook(target):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,target,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c645b1e if dispatcher=='primary' else 0x8c645b68
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{0x8c010000+i*2:hook(0x8c010000+i*2) for i in range(depth)},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f),(depth,mutation,bank,mask)
  assert not errors
  indices=list(range(depth)) if mutation==0 else ([0]+list(range(2,depth)) if mutation==2 and depth>=3 else [0] if depth else [])
  assert [x[0] for x in actual]==[0x8c010000+i*2 for i in indices]
  assert [x[1] for x in actual]==[0x12340000+i for i in indices]
  if actual:assert actual[0][2]&0xf0==0x60
  else:assert sr.value&0xf0==0x60
  count+=1
 report=dict(cases_passed=count,instruction_counts={'primary':19,'secondary':19},binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={name:hashlib.sha256((out/'source'/('sa_interrupt_dispatch_'+name+'.c')).read_bytes()).hexdigest() for name in ['primary','secondary']},scope='Full retail comparison for primary and secondary interrupt callback traversal; independent order, payload and IMASK=6 assertions; callback mutation of next link, empty queues, both FPU banks. Gameplay callbacks injected; generic interrupt dispatch and portable scheduling unfinished.')
 (out/'analysis/interrupt_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'interrupt dispatch comparisons passed')
if __name__=='__main__':main()
