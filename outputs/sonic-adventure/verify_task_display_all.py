import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_display_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c010000,0x8c010002]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
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
 for name,entry in [('sa_task_display_all_native',0x8c0988c4)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product(range(32),range(9),range(5),[0,0x200000]):
   for mem in [ram,native]:
    mem[0xa00000:0xa01000]=bytes(4096);mem[0xefff80:0xf00000]=bytes(128)
    for priority in range(8):
     base=0x8ca00100+priority*256
     struct.pack_into('<I',mem,0x79f04c+priority*4,base if alias and (stage==8 or priority==stage) else 0)
     for i in range(alias):
      struct.pack_into('<I',mem,(base&0xffffff)+i*64,base+(i+1)*64 if i+1<alias else 0)
      struct.pack_into('<I',mem,(base&0xffffff)+20+i*64,(0x8c010002 if variant&16 else 0x8c010000) if variant&(1<<i) else 0)
    struct.pack_into('<I',mem,0x1c1d40,0xdeadbeef)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=stage;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   ordered=[0x8ca00100+priority*256+i*64 for priority in ([7,0,1,2,3,4,5,6] if stage==8 else [stage]) for i in range(alias) if variant&(1<<i)]
   assert [t[1][0] for t in actual]==ordered
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=75,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_task_display_all','sa_task_display_list','sa_task_display_children','task_display_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='All-priority display dispatcher connected to native task display lists, priority7 first then0..6: all8 priorities and priority8 alias7, empty to4-node lists, null/callable callbacks, cached-next traversal despite callback link mutation, active callback global and clearing. Full regs/SR/PR/FPU/FPUL/RAM/call order compared against retail instructions; independent callback counts and multi-priority order assertions. Callback bodies injected; native callback integration pending.')
 (out/'analysis/task_display_all_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'all-priority native task display comparisons passed')
if __name__=='__main__':main()
