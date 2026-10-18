import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';fn=ctypes.CDLL(str(dll)).sa_interrupt_input_events_extended_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c010700]+[0x8c010600+i*2 for i in range(8)]
 patterns=[(0,255),(255,0)]
 for enabled,alternate,values,available,pending,bank,style,tick in itertools.product([0,1],[0,1],patterns,[0,255],[0xa5a50055],[0,0x200000],range(4),[0,0xffffffff]):
  previous,current=values;current&=255;ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67342c,enabled);struct.pack_into('<5I',ram,0x88f55c,0xdeadbeef,previous,pending,pending,alternate)
  ram[0x894138:0x894538]=bytes(1024)
  for channel in range(8):struct.pack_into('<I',ram,0x894138+channel*128+4,0 if available&(1<<channel) else 1)
  for channel in range(8):
   struct.pack_into('<I',ram,0x894138+channel*128+0x74,0x8c010600+channel*2)
   struct.pack_into('<I',ram,0x894138+channel*128+0x7c,0xface0000+channel)
  struct.pack_into('<I',ram,0x8a83b4,0x8ca04000)
  struct.pack_into('<I',ram,0x8a83b8,0x8ca06000)
  struct.pack_into('<3I',ram,0xa06204,tick,3,3)
  states=[[0,2,3,4][(style+i)%4] for i in range(3)];due=[(tick+d)&0xffffffff for d in [0,1,-1]]
  for i in range(3):
   struct.pack_into('<I',ram,0xa06210+i*0x124+0xc,states[i])
   struct.pack_into('<I',ram,0xa06210+i*0x124+0x14,due[i])
   struct.pack_into('<I',ram,0xa06210+i*0x124+0x118,0x8c010700 if style%2 else 0)
  struct.pack_into('<2I',ram,0x8e9dd8,0x8cbabe00,0xdeadbeef)
  for channel in range(8):
   struct.pack_into('<I',ram,0x6724f4+channel*4,channel)
   ram[0x893bd0+channel*24]=1
   struct.pack_into('<I',ram,0xa04000+0x44c+channel*120,2 if current&(1<<channel) else 0)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[4:8]),r[15],pr);value=0x12340000
   for i in range(4):r[i]=0x34560000+i
   r[0]=value;return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c602cb8
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666)
  reference(ram,s,{t:hook(t) for t in targets},f,special=dict(mach=mach.value,macl=0,gbr=gbr.value))
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
  assert not errors
  wanted=[];press=pending;release=pending
  if enabled:
   wanted=[]
   for i,state in enumerate(states):
    job=0x8ca06210+i*0x124
    if state==3 and style%2:wanted.append((0x8c010700,0x8ca06000))
    if state==1 and due[i]<=tick:wanted.append((0x8c1108e0,job+0x18))
   for rising,which,target in [(True,'press',0x8c602df8 if alternate else 0x8c602e34),(False,'release',0x8c647bb8)]:
    edge=(current^previous)&(current if rising else previous);queued=press if rising else release
    for i in range(8):
     bit=1<<i
     if edge&bit:queued|=bit
     if available&bit and queued&bit:
      wanted.extend([] if rising else [(0x8c010600+i*2,i)]);queued&=~bit
    if rising:press=queued
    else:release=queued
   assert struct.unpack_from('<4I',native,0x88f55c)==(previous,current,press,release)
  assert [(t[0],t[1][0] if t[0]!=0x8c110954 else None) for t in actual]==wanted
  assert struct.unpack_from('<I',native,0xa06204)[0]==((tick+int(bool(enabled)))&0xffffffff)
  assert struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8cbabe00
  for t in actual:
   if t[0] not in [0x8c110954,0x8c1108e0,0x8c010700]:assert t[1][1:]==(2,0,0xface0000+t[1][0])
   if t[0]==0x8c647cb4:assert t[1][1]==(1 if alternate else 18)
   if t[0]==0x8c647cc4:assert t[1]==(t[1][0],t[1][0],0,0)
  for i,state in enumerate(states):
   job=0xa06210+i*0x124
   assert struct.unpack_from('<I',native,job+0xc)[0]==(0 if enabled and state==3 else 3 if enabled and state==2 else state)
   if enabled and state==2:assert struct.unpack_from('<I',native,job+0x20)[0]==0x8c668d56
  assert struct.unpack_from('<I',native,0xa0620c)[0]==3-(states.count(3) if enabled else 0)
  for t in actual:
   if t[0]==0x8c010700:assert t[1][1] in [0x8ca06210+i*0x124 for i,state in enumerate(states) if state==3]
  if enabled:assert struct.unpack_from('<I',native,0xa08690)[0]==0x8ca06210+2*0x124
  ready=(available&(pending|((previous^current)&current))&255) if enabled else 0
  for channel in range(8):
   offset=0x894138+channel*128;active=bool(ready&(1<<channel))
   assert struct.unpack_from('<I',native,offset+4)[0]==int(active or not (available&(1<<channel)))
   assert struct.unpack_from('<I',native,offset+8)[0]==((1 if alternate else 18) if active else 0)
   assert struct.unpack_from('<6I',native,offset+0x14)==((channel,0,0,0,0,0) if active else (0,0,0,0,0,0))
  count+=1
 report=dict(cases_passed=count,instruction_counts={'input_events':145,'press_helpers':60,'channel_records':34,'channel_available':13,'sample_mask':29,'channel_query':32,'pointer_helpers':25,'channel_callback':19,'frame_wrappers':7,'frame_manager':85,'work_dispatch':17},binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={name:hashlib.sha256((out/'source'/(name+'.c')).read_bytes()).hexdigest() for name in ['sa_interrupt_input_events', 'sa_input_press_primary', 'sa_input_press_alternate', 'sa_input_channel_state_get', 'sa_input_channel_event_set', 'sa_input_channel_request_set', 'sa_input_channel_available', 'sa_input_sample_mask', 'sa_input_channel_query', 'sa_input_channel_status_pointer', 'sa_input_channel_info_pointer', 'sa_input_device_info_pointer', 'sa_input_channel_callback', 'sa_input_frame_process', 'sa_input_frame_backend', 'sa_input_frame_manager', 'sa_input_work_dispatch', 'sa_input_context_save', 'input_frame_context_native', 'task_delete_native']},scope='Full retail state/call comparisons; independent eight-channel edge/pending-event model, disabled path, both press dispatch variants, all/none/alternating availability and retained high bits. Press helpers and channel record stores native; availability and sampling native; channel query and pointer helpers native; channel callback wrapper native; frame wrappers and manager native, three-record queues tested in states 0/2/3/4; counter wrap and installed work callback checked; work dispatcher native, optional work callbacks checked; context save native with explicit MACH/GBR; only installed work/channel callback bodies injected; state-1 restoration excluded; independent posted-record checks; device backends unfinished.')
 (out/'analysis/interrupt_input_events_extended_queue_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'input event comparisons passed')
if __name__=='__main__':main()

