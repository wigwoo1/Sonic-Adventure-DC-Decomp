import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll'));counts={k:0 for k in ['pool','object','work','schedule']}
 for channel,fill,bank in itertools.product(range(8),[0x5a],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x8d6938:0x8e9de4]=bytes([fill])*(0x134a0+12)
  ram[0x894118:0x89c538]=bytes([0x5a])*(0x89c538-0x894118)
  work_entry=0x8c647980 if channel==7 else 0x8c64790e+channel*4
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank;f.fpul=0xabcdef01;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  def run(name,entry,args,stop=0x8c000100,result=0):
   fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call]
   for i,v in args.items():s.r[i]=v;r[i]=v
   s.pc=entry;s.pr=0x8c000100;pr.value=s.pr
   special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value)
   reference(ram,s,fpu=f,special=special,stop_pc=stop)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==result
   nf.fpul=fpul.value
   assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   assert [mach.value,macl.value,gbr.value]==[special[k] for k in ['mach','macl','gbr']]
  run('sa_input_channels_initialize_native',0x8c6477cc,{4:1<<channel})
  assert r[0]==0;obj=struct.unpack_from('<I',native,0x8a83b8)[0]
  assert obj==0x8c8d6938 and struct.unpack_from('<I',native,0x8e9de0)[0]==1
  assert native[0x894118:0x894538]==bytes(1056)
  for i in range(8):assert native[0x894538+i*4096:0x894538+(i+1)*4096]==bytes([0x30+i])*4096
  assert struct.unpack_from('<I',native,(obj&0xffffff)+0x210+0x20)[0]==work_entry
  counts['pool']+=1;counts['object']+=1;counts['work']+=1
  record=0x89c550+channel*0x148;offset=0x894138+channel*128
  for mem in [ram,native]:
   mem[record:record+0x148]=bytes(0x148);mem[record+2]=1;struct.pack_into('<I',mem,offset+8,18);struct.pack_into('<I',mem,offset+0x14,channel);struct.pack_into('<I',mem,0x894114,0);struct.pack_into('<I',mem,0x66d3a0,0);struct.pack_into('<I',mem,0x6724f4+channel*4,channel);struct.pack_into('<I',mem,0x8a83b4,0x8ca11000);mem[0x893bd0+channel*24]=0
  for frame in range(3):
   run('sa_input_frame_backend_native',0x8c668e60,{4:obj})
   assert pr.value==0x8c000100 and r[15]==0x8cf00000
   assert struct.unpack_from('<I',native,0x8d6938+0x204)[0]==frame+1
   assert struct.unpack_from('<I',native,0x8d6938+0x210+0x14)[0]==frame+1
   assert struct.unpack_from('<I',native,0x8d6938+0x210+0x20)[0]==0x8c668e48
   if frame>=1:assert struct.unpack_from('<I',native,offset+8)[0]==0 and struct.unpack_from('<I',native,0x894118+channel*4)[0]==0xffffff01
   counts['schedule']+=1
 report=dict(cases_passed=counts,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_channels_initialize','sa_input_work_backend_initialize','sa_input_work_backend_post','sa_input_work_pool_initialize','sa_input_work_object_create','sa_input_work_create','sa_input_frame_manager','sa_input_context_save','sa_input_context_restore','sa_input_channel_worker','sa_input_work_yield','sa_memory_move','sa_memory_fill','input_frame_context_native','sa_input_backend_request','sa_input_device_request','sa_input_press_service','sa_input_channel_secondary_callback','sa_input_event_error','sa_input_channel_status_pointer','sa_input_channel_info_pointer','sa_input_device_info_pointer']},scope='Native channel initialization creates actual worker and scheduler contexts. Event 18 then executes nested backend yield, restored request/press/worker returns, cleanup and idle yields over three frames across eight channels and FPU banks. Full retail RAM/CPU/FPU/special comparisons each stage. No injected calls or constructed contexts; event and absent-device tables seeded as fixtures. Full interrupt input and present-device paths unfinished.')
 (out/'analysis/input_channels_initialized_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'initialized backend lifecycle comparisons passed')
if __name__=='__main__':main()
