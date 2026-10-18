import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_channel_worker_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,event,tick,bank,savedbank,kind,stage,limit in itertools.product(range(8),[18],[0,0xffffffff],[0,0x200000],[0,0x200000],range(5),range(4),[0,2]):
  rng=random.Random(channel^tick);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0x8e9dd8,0x8ca00000,0x8cab0000);struct.pack_into('<2I',ram,0xa00204,tick,1);struct.pack_into('<I',ram,0xa02690,0x8ca03000)
  words=[rng.getrandbits(32) for i in range(38)];words[0]=0x8cefffe0;words[1]=0x8ca03000;words[2]=0x8c668ef2;words[3]=0x8ca00000;words[4]=0;words[5]=3;words[6]=0x8c1108e0;words[7]=0x8c8e9dd8;words[37]=savedbank|0x40001;struct.pack_into('<38I',ram,0xa00004,*words)
  struct.pack_into('<8I',ram,0xefffe0,bank,0x8c000100,*[0x22340000+i for i in range(9,15)])
  offset=0x894138+channel*128;ram[offset:offset+128]=bytes(128);struct.pack_into('<I',ram,offset+8,event);struct.pack_into('<I',ram,offset+0x14,channel)
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000);struct.pack_into('<I',ram,0xa10008,0);record=0x89c550+channel*0x148;ram[record:record+0x148]=bytes(0x148);ram[record+2]=1;struct.pack_into('<I',ram,record+4,kind);struct.pack_into('<I',ram,0x673bec+kind*80,11);struct.pack_into('<I',ram,0x8a0fb0+channel*0x4c+0x1c,0x8ca02000)
  struct.pack_into('<I',ram,0x894114,limit);struct.pack_into('<I',ram,0x66d3a0,0)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank|0x40001;f.fpul=0xabcdef01
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cee0000;s.sr=0x70000063;s.pr=0x8c010600;s.pc=0x8c647982
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value);errors=[];actual=[];expected=[]
  targets=[0x8c6608a4,0x8c64b4e4,0x8c660a7e]
  def effect(r,t):
   trace=(t,tuple(r[i] for i in range(4,8)));r[0]=(0xffffff01 if t==0x8c6608a4 else 0xffffffff) if stage==targets.index(t)+1 else 0;return trace
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t));return sr
   return body
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t));return 0
  reference(ram,s,{t:hook(t) for t in targets},fpu=f,special=special,stop_pc=0x8c000100)
  result=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call);assert result==0,(result,errors,channel,event)
  nf.fpul=fpul.value
  assert actual==expected and not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert [mach.value,macl.value,gbr.value]==words[17:20] and pr.value==0x8c000100 and r[15]==0x8cf00000
  assert struct.unpack_from('<I',native,0xa03014)[0]==((tick+1)&0xffffffff)
  assert struct.unpack_from('<I',native,0xa03020)[0]==0x8c668e48
  assert struct.unpack_from('<I',native,0xa00204)[0]==((tick+1)&0xffffffff) and struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8cab0000
  wanted=0xffffff01 if stage in [1,3] else 0xffffff83 if stage==2 else [15,33,65,122,0xffffff83][kind]
  assert struct.unpack_from('<I',native,0x894118+channel*4)[0]==wanted and struct.unpack_from('<I',native,offset+8)[0]==0
  count+=1
 report=dict(cases_passed=count,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_frame_manager','sa_input_context_save','sa_input_context_restore','sa_input_work_yield','sa_input_channel_worker','input_frame_context_native','sa_input_event_service','sa_input_event_error','sa_input_press_service','sa_input_channel_secondary_callback','sa_input_device_request']},scope='Full retail native channel-worker/yield/save/manager-restore/manager-resume comparison across eight channels and event 18, five device types and three backend error stages; three lower backend services injected, with native retry/request wrapper and retry limits zero/two. Due-time, saved work continuation, special/FPU/register state, manager caller return and counter wrap checked. Manager saved context fixture constructed; resuming suspended work and complete input graph integration unfinished.')
 (out/'analysis/input_channel_worker_request_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native worker request comparisons passed')
if __name__=='__main__':main()


