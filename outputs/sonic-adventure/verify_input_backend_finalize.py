import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_backend_finalize.dll')).sa_input_backend_finalize
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c660dec,0x8c646856,0x8c646b36,0x8c6469c8,0x8c646884,0x8c66086a,0x8c6607fc]
 for channel,present,rows,packet,busy,wait,bank in itertools.product([0,7],[0,1],[0,1,3],[8,0,251],[0,1],[0,1],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;record=0x89c550+channel*0x148;struct.pack_into('<I',ram,record+28,0x8ca01000);struct.pack_into('<I',ram,0xa0103c,rows);struct.pack_into('<I',ram,0xa01044,16);struct.pack_into('<I',ram,0x6724f4+channel*4,channel);ram[0xa00004]=packet;struct.pack_into('<I',ram,0xa00008,0x8ca03000);ram[0xa0300c:0xa0301c]=bytes(range(16))
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];indices=[[0,0],[0,0]]
  def effect(r,t,pr,side,mem):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr)
   if t==0x8c660dec:r[0]=present
   elif t==0x8c646856:r[0]=0x8ca00000
   elif t==0x8c646b36:r[0]=busy
   elif t==0x8c6469c8:r[0]=0xffffffff if indices[side][0]<wait else 7;indices[side][0]+=1
   elif t==0x8c646884:r[0]=0 if indices[side][1]<wait else 1;indices[side][1]+=1
   elif t==0x8c6607fc:
    dst=r[4]&0xffffff;src=r[5]&0xffffff;mem[dst:dst+r[6]]=mem[src:src+r[6]];r[0]=0
   else:r[0]=0
   return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0],1,native));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr,0,mem));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[5]=0;s.r[6]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c660a7e
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  result=0xffffff01 if not present else 0 if rows==0 else 0xffffff30 if busy else 0 if packet==8 else 0xffff0000 if packet==251 else 0xffffff01
  assert r[0]==result
  copies=[x for x in actual if x[0]==0x8c6607fc]
  wanted=rows if present and not busy and packet==8 else 0
  assert len(copies)==wanted
  for i,x in enumerate(copies):assert x[1][:3]==(0x8ca02000+i*16,0x8ca0300c,16)
  if wanted:assert native[0xa02000:0xa02000+rows*16]==bytes(range(16))*rows
  count+=1
 report=dict(cases_passed=count,instruction_count=130,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_backend_finalize.c').read_bytes()).hexdigest(),scope='Retail full state/call comparison of absent device, zero/multiple rows, busy status, allocation/completion waits and packet success/error codes. Independent results/copy count/offset/output checks. Seven lower services injected including synchronous yield and packet copy; actual suspension unfinished.')
 (out/'analysis/input_backend_finalize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'backend finalize comparisons passed')
if __name__=='__main__':main()
