import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_pop.dll'))
 pop=lib.sa_engine_queue_pop
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll')).sa_engine_frame_services
 pending=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll')).sa_engine_pending_count_get
 error_fn=lib.sa_engine_error_record
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 error_fn.argtypes=fn.argtypes;pop.argtypes=fn.argtypes;pending.argtypes=fn.argtypes
 cases=0
 for index,count,mode,bank,mask in itertools.product([0,3],[0,1,4],[0,1,2],[0,0x200000],[0,7,15]):
  present=True;capacity=4
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca00200 if present else 0)
  struct.pack_into('<5I',ram,0xa00394,0x8ca00400,capacity,2,index,count)
  struct.pack_into('<2I',ram,0xa00400+index*8,0x8ca02000,0x76543210)
  struct.pack_into('<2I',ram,0xa00000,0x12345678,0xabcdef01)
  struct.pack_into('<I',ram,0xa00388,0)
  struct.pack_into('<I',ram,0xa003a8,mode)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[];actual=[];expected=[]
  targets=[0x8c65a518,0x8c65a5dc,0x8c666d10]
  hooks={}
  for t in targets:
   def hook(m,r,sr,t=t):expected.append((t,tuple(r[4:6])));r[0]=0;return sr
   hooks[t]=hook
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c666f98:return pop(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t==0x8c66abfc:return pending(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t in targets:actual.append((t,tuple(r[4:6])));r[0]=0;return 0
   if t==0x8c65acac:return error_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c668f40;s.r[4]=0x8ca00000;s.r[5]=0x8ca00004
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,hooks=hooks,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert actual==expected and actual[0][0]==targets[0] and actual[0][1][0]==0x8ca00000
  assert [e[0] for e in actual]==[targets[0]]+([targets[1+mode]] if count and mode in [0,1] else [])
  if count:
   assert struct.unpack_from('<2I',native,0xa00400+index*8)==(0,0)
   assert struct.unpack_from('<2I',native,0xa003a0)==((index+1)%capacity,count-1)
   if mode in [0,1]:assert actual[-1][1]==(0x8ca02000,0x76543210)
  else:assert struct.unpack_from('<2I',native,0xa003a0)==(index,0)
  assert r[15]==0x8cf00000
  cases+=1
 (out/'analysis/engine_frame_services_queue_routing_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=110,scope='Full retail RAM/CPU/SR/PR/FPU/event equality with native pending count getter/queue pop/error recorder. Empty/one/full queue, index wrap, modes0/1/2, both banks/three masks. Independent pop/erasure/count/index and direct versus context consumer arguments. Callback dispatch and consumers modeled; no transfer completion or scheduler proof.'),indent=2)+'\n')
 print(cases,'queue pop comparisons passed')
if __name__=='__main__':main()

