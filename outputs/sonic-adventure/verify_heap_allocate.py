import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/heap_allocate.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  r[0]=0x8ca00300 if alias&1 else 0
  for i in range(1,4):r[i]=0x12340000+i
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);actual.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
  return body
 for name,entry in [('sa_heap_allocate',0x8c098a80)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product([0,1,4,15,16,17,31,32,33,47,48,63,64,127,128,255,256,513],[0,16,32,48,80,128,512],range(3),[0,0x200000]):
   for mem in [ram,native]:
    mem[0xa00000:0xa02000]=bytes([0xa5])*8192;mem[0xefffc0:0xf00000]=bytes(64)
    struct.pack_into('<I',mem,0x79f03c,0x8ca00100 if alias else 0)
    struct.pack_into('<4I',mem,0xa00100,0x8ca00900 if alias==2 else 0,0,0x8ca00100+stage if stage else 0,0)
    if stage:struct.pack_into('<4I',mem,0xa00100+stage,0,0,0,0x8ca00100)
    struct.pack_into('<4I',mem,0xa00900,0,0x8ca00100,0x8ca00a00,0)
    struct.pack_into('<4I',mem,0xa00a00,0,0,0,0x8ca00900)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=variant;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   need=(variant+31)&0xfffffff0
   chosen=0
   if variant and alias and stage:
    if stage>=need:chosen=0x8ca00100
    elif alias==2 and 256>=need:chosen=0x8ca00900
   assert r[0]==(chosen+16 if chosen else 0)
   if chosen:
    assert struct.unpack_from('<2I',native,chosen&0xffffff)==(0,0)
    capacity=stage if chosen==0x8ca00100 else 256
    if capacity>need:
     assert struct.unpack_from('<I',native,(chosen&0xffffff)+8)[0]==chosen+need
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=70,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in counts},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Whole core heap allocator: zero requests, empty lists, physical sentinel, exact fits, split fits, first-block miss/second-block match and exhaustion. Full-state retail comparisons plus independent first-fit result/header/split assertions. Both banks. Valid finite heaps; overflow request arithmetic not yet covered. Caller integration pending.')
 (out/'analysis/heap_allocate_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'heap allocation comparisons passed')
if __name__=='__main__':main()
