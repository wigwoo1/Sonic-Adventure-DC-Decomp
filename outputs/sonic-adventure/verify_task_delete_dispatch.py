import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  assert struct.unpack_from('<I',mem,0x1c1d40)[0]==target
  if target==0x8c010002:struct.pack_into('<I',mem,r[4]&0xffffff,0)
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
 for name,entry in [('sa_task_delete_regular_native',0x8c0988e4),('sa_task_delete_all_native',0x8c098900)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product(range(32),range(9),range(5),[0,0x200000]):
   for mem in [ram,native]:
    mem[0xa00000:0xa01000]=bytes(4096);mem[0xefff80:0xf00000]=bytes(128)
    for priority in range(8):
     base=0x8ca00100+priority*256
     struct.pack_into('<I',mem,0x79f04c+priority*4,base if alias and (stage==8 or priority==stage) else 0)
     for i in range(alias):
      struct.pack_into('<2I',mem,(base&0xffffff)+i*64,base+(i+1)*64 if i+1<alias else 0,base+(i-1)*64 if i else 0)
      struct.pack_into('<I',mem,(base&0xffffff)+16+i*64,0x8c010000 if variant&(1<<i) else 0)
    struct.pack_into('<I',mem,0x79f048,0)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=stage;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   assert not actual
   limit=7 if 'regular' in name else 8
   deleted=[]
   def chain(head):
    result=[]
    while head:
     assert head not in result
     result.append(head);head=struct.unpack_from('<I',native,head&0xffffff)[0]
    return result
   for priority in range(8):
    active=stage==8 or stage==priority
    nodes=[0x8ca00100+priority*256+i*64 for i in range(alias)] if active else []
    selected=[node for i,node in enumerate(nodes) if priority<limit and variant&(1<<i)]
    deleted+=selected
    retained=[node for node in nodes if node not in selected]
    assert chain(struct.unpack_from('<I',native,0x79f04c+priority*4)[0])==retained
   assert chain(struct.unpack_from('<I',native,0x79f048)[0])==list(reversed(deleted))
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=54,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_task_delete_regular','sa_task_delete_all','sa_task_delete_list','sa_task_delete','task_delete_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Native regular-priority and all-priority cleanup dispatchers connected to list traversal/deletion.0..4 tasks per priority, all8 lists active together or each individually, callback presence/null patterns,both banks. No service hooks. Full-state retail comparisons and independent every-priority retained-list and global reverse deletion-order free-list assertions. Startup integration incomplete.')
 (out/'analysis/task_delete_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'native cleanup dispatcher comparisons passed')
if __name__=='__main__':main()
