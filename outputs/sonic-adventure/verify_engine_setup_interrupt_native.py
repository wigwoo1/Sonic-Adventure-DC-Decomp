import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_interrupt_constructor import IO,Read,Write

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c60526a,0x8c099750,0x8c055c44,0x8c043998]
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
 for name,entry in [('sa_engine_setup_interrupt_native',0x8c08f210)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product([0,0x5a,0xa5],[0],[0],[0,0x200000]):
   for mem in [ram,native]:
    mem[0x7ee8df:0x87e8f1]=bytes([variant])*0x90012;mem[0xefffc0:0xf00000]=bytes(64)
    struct.pack_into('<2I',mem,0x79f03c,0xdeadbeef,0xdeadbeef)
    struct.pack_into('<I',mem,0x67c004,0x8ca00000)
    struct.pack_into('<I',mem,0x760970,0)
    struct.pack_into('<2I',mem,0x67c1d0,0,0)
    struct.pack_into('<I',mem,0x67342c,0)
    struct.pack_into('<I',mem,0x782974,0)
    struct.pack_into('<I',mem,0x161cb0,0)
    struct.pack_into('<I',mem,0x673420,0)
    struct.pack_into('<2h',mem,0x7493ba,0,0)
    mem[0xa00230:0xa00280]=bytes(80)
    struct.pack_into('<I',mem,0x8900fc,(1 if variant==0xa5 else 7) if variant else 0)
    mem[0x892840:0x893840]=bytes(4096)
    mem[0x893840:0x894040]=bytes(2048)
    if variant==0x5a:
     for i in range(256):struct.pack_into('<I',mem,0x892840+i*16+12,0x80000000)
    if variant==0xa5:
     struct.pack_into('<4I',mem,0x892850,0x8c010000,0,0,0x80000058)
     struct.pack_into('<I',mem,0x893a80,0x8c892850)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=stage;s.r[5]=0x8c07d560 if variant else 0;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   io=IO(0x8ca08000,0,None,Read(),Write())
   reference(ram,s,{t:hook(t) for t in targets},f,vbr=io.vbr)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   assert struct.unpack_from('<I',native,0x79f044)[0]==0x8c7ee8f0
   assert struct.unpack_from('<I',native,0x79f048)[0]==0x8c7ee8f0
   for i in range(4096):
    address=0x8c7ee8f0+i*48
    assert struct.unpack_from('<12I',native,address&0xffffff)==(address+48 if i<4095 else 0,address-48 if i else 0,0,0,0,0,0,0,0,0,0,0)
   assert struct.unpack_from('<I',native,0x79f03c)[0]==0x8c81e8f0
   assert struct.unpack_from('<I',native,0x7997b4)[0]==1 and struct.unpack_from('<I',native,0x7997bc)[0]==0
   assert struct.unpack_from('<4I',native,0x754ebc)==(1,1,1,0)
   assert struct.unpack_from('<I',native,0x8900fc)[0]==(0 if variant==0x5a else 1)
   assert [t[0] for t in actual]==[0x8c60526a,0x8c099750,0x8c055c44,0x8c043998]
   if variant!=0x5a:
    assert struct.unpack_from('<2I',native,0xa00238)==(0x8c645b1e,0)
    assert struct.unpack_from('<4I',native,0x892850)==(0x8c051810,0,0,0x80000058)
    assert struct.unpack_from('<I',native,0x893a80)[0]==0x8c892850
   assert struct.unpack_from('<I',native,0x7a0a80)[0]==0
   expected.clear();actual.clear();s.pc=0x8c645b1e
   reference(ram,s,{},f)
   dispatch=lib.sa_interrupt_dispatch_primary_native
   dispatch.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
   assert dispatch(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected
   assert [x[0] for x in actual]==[]
   assert struct.unpack_from('<I',native,0x754ec0)[0]==(1 if variant==0x5a else 0)
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=46,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_interrupt_job_query','sa_interrupt_job_dispatch','sa_interrupt_tail_dispatch','sa_stage_act_key','sa_interrupt_gameplay_callback','sa_interrupt_dispatch_primary','sa_interrupt_dispatch_secondary','sa_interrupt_handler_primary','sa_interrupt_handler_secondary','sa_interrupt_handler_set','sa_interrupt_handler_reset','sa_interrupt_callback_remove','sa_interrupt_callback_create','sa_setup_callback_register','sa_setup_callback_state','sa_setup_counter_set','sa_engine_setup','sa_heap_initialize','sa_task_pool_initialize','sa_heap_allocate','task_delete_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Original engine setup caller connected to native full heap initialization, core allocation and4096-record task pool initialization. Full-state retail comparisons and independent pool/global/call-order assertions,three prior memory patterns,both banks. Interrupt constructor and removal native, including success, replacement and pool exhaustion; handler installation and reset native, remaining initialization services injected; Registered primary dispatcher also executed from the resulting setup queues; gameplay callback native; its all fixed direct calls native; job wrappers native; default nullable backend path has no injected dispatch calls; input event routine native in disabled path; 0x8c643f98 native with null-object path; stage/act getter native. Complete startup/game loop incomplete.')
 (out/'analysis/engine_setup_interrupt_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'engine setup comparisons passed')
if __name__=='__main__':main()

