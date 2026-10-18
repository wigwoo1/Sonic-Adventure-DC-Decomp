import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_backend_request_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,event,tick,bank,savedbank,allocation_wait,completion_wait,busy in itertools.product(range(8),[0],[0,0xffffffff],[0,0x200000],[0,0x200000],[1,2],[0,2],[0,1]):
  rng=random.Random(channel^tick);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0x8e9dd8,0x8ca00000,0x8cab0000);struct.pack_into('<2I',ram,0xa00204,tick,1);struct.pack_into('<I',ram,0xa02690,0x8ca03000)
  words=[rng.getrandbits(32) for i in range(38)];words[0]=0x8cefffe0;words[1]=0x8ca03000;words[2]=0x8c668ef2;words[3]=0x8ca00000;words[4]=0;words[5]=3;words[6]=0x8c1108e0;words[7]=0x8c8e9dd8;words[37]=savedbank|0x40001;struct.pack_into('<38I',ram,0xa00004,*words)
  struct.pack_into('<8I',ram,0xefffe0,bank,0x8c000100,*[0x22340000+i for i in range(9,15)])
  offset=0x894138+channel*128;ram[offset:offset+128]=bytes(128);struct.pack_into('<I',ram,offset+8,event)
  struct.pack_into('<I',ram,0x6724f4+channel*4,channel);ram[0xa10000]=1;struct.pack_into('<I',ram,0xa11000,2)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank|0x40001;f.fpul=0xabcdef01
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cee0000;s.sr=0x70000063;s.pr=0x8c010600;s.pc=0x8c6608a4;s.r[5]=0x8ca20000
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value);errors=[];indices=[[0,0],[0,0]]
  targets=[0x8c646856,0x8c646868,0x8c646b3c,0x8c646982,0x8c646884,0x8c646b36]
  def effect(r,t,side):
   if t==0x8c646856:r[0]=0x8ca10000
   elif t==0x8c646868:r[0]=0x8ca11000
   elif t==0x8c646982:r[0]=0xffffffff if indices[side][0]<allocation_wait else 7;indices[side][0]+=1
   elif t==0x8c646884:r[0]=0 if indices[side][1]<completion_wait else 1;indices[side][1]+=1
   elif t==0x8c646b36:r[0]=busy
   else:r[0]=0
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   effect(r,t,1);return 0
  def hook(t):
   def body(mem,r,sr):effect(r,t,0);return sr
   return body
  hooks={t:hook(t) for t in targets}
  reference(ram,s,hooks,fpu=f,special=special,stop_pc=0x8c000100)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  nf.fpul=fpul.value
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert [mach.value,macl.value,gbr.value]==words[17:20] and pr.value==0x8c000100 and r[15]==0x8cf00000
  assert struct.unpack_from('<I',native,0xa03014)[0]==((tick+1)&0xffffffff)
  assert struct.unpack_from('<I',native,0xa03020)[0]==0x8c668e48
  assert struct.unpack_from('<I',native,0xa00204)[0]==((tick+1)&0xffffffff) and struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8cab0000
  restore=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_context_restore_native;restore.argtypes=fn.argtypes
  yields=allocation_wait+completion_wait+busy
  for step in range(yields):
   struct.pack_into('<I',ram,0x8e9dd8,0x8ca00000);struct.pack_into('<I',native,0x8e9dd8,0x8ca00000)
   s.r[4]=0x8ca03018;s.r[5]=1;s.pr=0x8c010600;s.pc=0x8c1108e0;r[4]=s.r[4];r[5]=1;pr.value=s.pr
   target=0x8c010600 if step==yields-1 else 0x8c000100
   reference(ram,s,hooks,fpu=f,special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value),stop_pc=target)
   assert restore(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
   nf.fpul=fpul.value
   assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==(0xffffff30 if busy else 0xffffff01) and r[15]==0x8cee0000 and pr.value==0x8c010600
  assert indices==[[allocation_wait+1,completion_wait+1]]*2
  assert struct.unpack_from('<I',native,0xa00204)[0]==((tick+yields)&0xffffffff)
  count+=1
 report=dict(cases_passed=count,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_frame_manager','sa_input_context_save','sa_input_context_restore','sa_input_work_yield','sa_input_channel_worker','input_frame_context_native','sa_input_backend_request']},scope='Repeated actual allocation/completion/busy yields, saved CPU/FPU context restoration, native backend continuation and final original caller return. Full retail state checked after every yield/restore across channels, banks and ticks. Six device services injected; manager context fixture. Nested worker/press frames unfinished.')
 (out/'analysis/input_backend_wait_context_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'backend wait saved-context comparisons passed')
if __name__=='__main__':main()
