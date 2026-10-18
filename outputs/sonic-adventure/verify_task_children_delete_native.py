import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c010000,0x8c010002]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  node=r[4]&0xffffff;next_node=struct.unpack_from('<I',mem,node)[0]
  if variant&2:struct.pack_into('<I',mem,0xa0008c,next_node)
  if variant&4:struct.pack_into('<I',mem,node,0)
  r[0]=0x12345678
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
 for name,entry in [('sa_task_children_delete_native',0x8c098e60)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product(range(8),[0],range(5),[0,0x200000]):
   for mem in [ram,native]:
    mem[0xa00000:0xa00400]=bytes(1024);mem[0xefff80:0xf00000]=bytes(128)
    struct.pack_into('<I',mem,0x79f048,0)
    struct.pack_into('<I',mem,0xa0008c,0x8ca00100 if alias else 0)
    struct.pack_into('<I',mem,0xa00090,0x8c098e60 if variant&1 else 0x8c010004)
    for i in range(alias):
     struct.pack_into('<3I',mem,0xa00100+i*64,0x8ca00100+(i+1)*64 if i+1<alias else 0,0x8ca00100+(i-1)*64 if i else 0,0x8ca00080)
     struct.pack_into('<I',mem,0xa00110+i*64,0x8c010000)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00080;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   assert len(actual)==(alias if variant&1 else 0)
   if variant&1:
    empty=not alias or bool(variant&2)
    assert struct.unpack_from('<I',native,0xa00090)[0]==(0x8c09865c if empty else 0x8c098e60)
    assert [t[1][0] for t in actual]==[0x8ca00100+i*64 for i in range(alias)]
   else:
    assert struct.unpack_from('<I',native,0xa0008c)[0]==0
    assert struct.unpack_from('<I',native,0x79f048)[0]==(0x8ca00100+(alias-1)*64 if alias else 0)
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=45,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_task_children_delete','sa_task_children_update','task_delete_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Native child deletion/deferred child update dispatch.0..4 child lists,both banks,child callback head removal/next-link mutation. Native immediate task deletion or cached-next update callbacks and parent deletion scheduling when empty. Full-state retail and independent counts/order/head/free-list/update-pointer assertions. Child update callbacks injected; engine integration incomplete.')
 (out/'analysis/task_children_delete_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'deferred child task comparisons passed')
if __name__=='__main__':main()
