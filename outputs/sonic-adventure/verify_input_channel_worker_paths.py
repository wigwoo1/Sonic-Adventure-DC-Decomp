import ctypes,hashlib,itertools,json,re,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_channel_worker.dll')).sa_input_channel_worker_entry
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 targets=set(int(x,16) for x in re.findall(r'COMPUTED_CALL:([0-9a-f]+)',(out/'analysis/instructions/input_frame_process.tsv').read_text()));targets.discard(0x8c66086a);count=0
 for channel,event,bank,result in itertools.product(range(8),range(22),[0,0x200000],[0,1,0xffffff01]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;offset=0x894138+channel*128;ram[offset:offset+128]=bytes(128);struct.pack_into('<I',ram,offset+8,event)
  for i in range(6):struct.pack_into('<I',ram,offset+0x14+i*4,0x8ca00000)
  struct.pack_into('<I',ram,0xa00028,0x8ca01000)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];yielded=[]
  def effect(r,t,pr):trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr);r[0]=result;return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c66086a:yielded.append((r[4],pr[0]));return 1
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0]));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c647982
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f,stop_pc=0x8c66086a)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==1
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  assert yielded==[(s.r[4],s.pr)]
  count+=1
 report=dict(cases_passed=count,instruction_count=255,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_channel_worker.c').read_bytes()).hexdigest(),scope='Retail full state/call comparison of all eight channel workers through first yield, 21 event codes plus default, both FPU banks and zero/positive/error service results. Subordinate event services injected with selected results; native yield integration and real device routines unfinished.')
 (out/'analysis/input_channel_worker_paths_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'channel worker comparisons passed')
if __name__=='__main__':main()
