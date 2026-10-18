import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/position_task_callback_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  r[0]=0x76543210
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
 for name,entry in [('sa_position_task_callback_native',0x8c07d560)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for stage,variant,alias,mode in itertools.product(range(256),[0,118,119,120,121,32767,32768,65534,65535],range(2),[0,0x200000,0x380003]):
   for mem in [ram,native]:
    mem[0xa00000:0xa00400]=bytes(1024);mem[0xefffc0:0xf00000]=bytes(64)
    struct.pack_into('<I',mem,0xa00320,0x8ca00200)
    struct.pack_into('<H',mem,0xa00206,variant);mem[0xa00208]=stage
    struct.pack_into('<I',mem,0x78c668+stage*4,0x8ca00100 if alias else 0)
    struct.pack_into('<H',mem,0xa00106,0xa55a)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00300;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   timer=(variant+1)&65535
   assert struct.unpack_from('<H',native,0xa00206)[0]==timer
   flags=0xa55a if not alias else ((0xa55a&0x7fff) if timer>120 else (0xa55a|0x8000))
   assert struct.unpack_from('<H',native,0xa00106)[0]==flags
   assert not actual
   assert struct.unpack_from('<I',native,0xa00310)[0]==(0x8c09865c if timer>120 else 0)
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=40,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_position_task_callback','sa_task_remove_request','position_task_callback_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Whole position task callback: all256 IDs, timer threshold and wraparound, linked pointer present/null, actual flag writes and destroy tail call. Independent timer/flag/call assertions plus full regs/SR/PR/FPU/FPUL/stack/RAM against original instructions. Task removal request native: callback replaced with retail deletion entry; host scheduler and actual deletion entry remain incomplete.')
 (out/'analysis/position_task_callback_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'position task callback comparisons passed')
if __name__=='__main__':main()
