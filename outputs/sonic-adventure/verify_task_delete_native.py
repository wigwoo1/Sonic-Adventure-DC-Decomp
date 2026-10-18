import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c010000]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
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
 for name,entry in [('sa_task_delete_native',0x8c09865c)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product(range(128),range(9),range(2),[0,0x200000]):
   for mem in [ram,native]:
    mem[0xa00000:0xa01000]=bytes(4096);mem[0xefff80:0xf00000]=bytes(128)
    struct.pack_into('<I',mem,0x79f03c,0)
    for off in [0xa00800,0xa00900,0xa00a00,0xa00b00,0xa00e40,0xa00100,0xa00180,0xa00200,0xa00ec0,0xa00580,0xa00680,0xa00780,0xa00e00,0xa00ea0]:
     struct.pack_into('<4I',mem,off-16,0,0,0x8ca00f00,0)
    nodes=[0x8ca00100,0x8ca00180,0x8ca00200][:stage%4]
    struct.pack_into('<I',mem,0xa00e80,nodes[0] if nodes else 0)
    struct.pack_into('<I',mem,0xa00e84,0x8ca00ec0 if alias else 0)
    struct.pack_into('<I',mem,0xa00e7c,0x3f800000)
    for i,node in enumerate(nodes):struct.pack_into('<I',mem,(node&0xffffff)+16,nodes[i+1] if i+1<len(nodes) else 0)
    secondary=[0x8ca00580,0x8ca00680,0x8ca00780][:((stage//4)%2)+(stage%2)*2]
    struct.pack_into('<2I',mem,0xa00e88,0x3f800000,secondary[0] if secondary else 0)
    for i,node in enumerate(secondary):struct.pack_into('<I',mem,(node&0xffffff)+12,secondary[i+1] if i+1<len(secondary) else 0)
    struct.pack_into('<H',mem,0xa00e02,0x20 if stage&4 else 0)
    struct.pack_into('<I',mem,0xa00e0c,0x8ca00ea0 if alias else 0)
    task=0x8ca00300
    for off,bit,ptr in [(0,0,0x8ca00400),(4,1,0x8ca00500),(8,2,0x8ca00600),(12,3,0x8ca00700),(24,4,0x8c010000),(32,5,0x8ca00800),(28,6,0x8ca00c00)]:
     struct.pack_into('<I',mem,0xa00300+off,ptr if variant&(1<<bit) else 0)
    for off,ptr in [(36,0x8ca00900),(40,0x8ca00a00),(44,0x8ca00b00)]:struct.pack_into('<I',mem,0xa00300+off,ptr if alias else 0)
    struct.pack_into('<I',mem,0xa00310,0x8c07d560);struct.pack_into('<I',mem,0xa00314,0x8c010100)
    struct.pack_into('<2I',mem,0xa00838,0x8ca00e00 if stage&1 else 0,0x8ca00e40 if stage&2 else 0)
    struct.pack_into('<H',mem,0xa00c02,0xa55b)
    for i in range(8):struct.pack_into('<I',mem,0x79f04c+i*4,task if i==stage else 0x8ca00e00)
    struct.pack_into('<I',mem,0xa00708,task);struct.pack_into('<I',mem,0xa0070c,0x8ca00880);struct.pack_into('<I',mem,0xa00888,0x8ca00700)
    struct.pack_into('<I',mem,0x79f048,0x8ca00d00 if alias else 0)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00300;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   assert struct.unpack_from('<I',native,0x79f048)[0]==0x8ca00300
   for off in [4,8,12,16,20,24,28,32,36,40,44]:assert struct.unpack_from('<I',native,0xa00300+off)[0]==0
   assert struct.unpack_from('<I',native,0xa00300)[0]==(0x8ca00700 if variant&8 else (0x8ca00d00 if alias else 0))
   if alias:assert struct.unpack_from('<I',native,0xa00d04)[0]==(0x8ca00880 if variant&8 else 0x8ca00300)
   if variant&8:assert struct.unpack_from('<I',native,0xa00704)[0]==0x8ca00300
   if variant&64:assert struct.unpack_from('<H',native,0xa00c02)[0]==0xa55a
   released=(([0x8ca00e90] if stage&4 and alias else [])+[0x8ca00df0] if variant&32 and stage&1 else [])+([n-16 for n in nodes]+([0x8ca00eb0] if alias else [])+[n-16 for n in secondary] if variant&32 and stage&2 else [])+([0x8ca00e30] if variant&32 and stage&2 else [])+([0x8ca007f0] if variant&32 else [])+([0x8ca009f0,0x8ca008f0,0x8ca00af0] if alias else [])
   cursor=struct.unpack_from('<I',native,0x79f03c)[0]
   chain=[]
   while cursor:
    assert cursor not in chain
    chain.append(cursor);cursor=struct.unpack_from('<I',native,cursor&0xffffff)[0]
   assert chain==list(reversed(released)),(variant,stage,alias,chain,released)
   if variant&32 and stage&2:
    assert struct.unpack_from('<5I',native,0xa00e7c)==(0,0,0,0,0)
   if variant&32 and stage&1:
    assert struct.unpack_from('<I',native,0xa00838)[0]==0
    if stage&4 and alias:assert struct.unpack_from('<I',native,0xa00e0c)[0]==0
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=400,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_task_delete','sa_task_children_delete','sa_task_children_update','sa_task_data_release','sa_heap_release','sa_task_optional_cleanup','sa_task_cleanup_list','sa_task_cleanup_secondary','sa_task_payload_cleanup','task_delete_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Whole119-instruction task deletion body: next/previous/parent/child links, destructor presence, payload/resource pointers, all8 active-list slots plus absent, empty/nonempty free list. Full registers/SR/PR/FPU/FPUL/stack/RAM/calls against retail instructions; independent free-list insertion, pointer clearing and resource flag assertions. Destructor, recursive child deletion native with child/grandchild fixtures; task payload release native; destructor, optional payload cleanup and heap release native; destructor and optional cleanup services injected; full scheduler remains incomplete.')
 (out/'analysis/task_delete_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'task deletion comparisons passed')
if __name__=='__main__':main()


