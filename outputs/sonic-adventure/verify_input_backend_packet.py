import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_backend_request.dll')).sa_input_backend_request
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c646856,0x8c646868,0x8c646b3c,0x8c646982,0x8c646884,0x8c646b36,0x8c66086a,0x8c646bbc,0x8c10ca5c,0x8ca05000]
 for channel,present,flags,allocation_wait,completion_wait,busy,packet,bank,width,height,decode_error in itertools.product(range(8),[1],[2],[0],[0],[0],[8],[0,0x200000],[1,512,65535],[1,513],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x6724f4+channel*4,channel);ram[0xa00000]=present;ram[0xa00004]=packet;struct.pack_into('<I',ram,0xa01000,flags)
  struct.pack_into('<I',ram,0xa00008,0x8ca03000);struct.pack_into('<I',ram,0x89c54c,0x8ca04000);struct.pack_into('<I',ram,0xa0400c,0x8ca05000)
  packetdata=bytes((i*17+channel)&255 for i in range(32));ram[0xa03008:0xa03028]=packetdata
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];indices=[[0,0],[0,0]]
  def effect(r,t,pr,side,mem):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr)
   if t==0x8c646856:r[0]=0x8ca00000
   elif t==0x8c646868:r[0]=0x8ca01000
   elif t==0x8c646982:r[0]=0xffffffff if indices[side][0]<allocation_wait else 7;indices[side][0]+=1
   elif t==0x8c646884:r[0]=0 if indices[side][1]<completion_wait else 1;indices[side][1]+=1
   elif t==0x8c646b36:r[0]=busy
   elif t==0x8ca05000:r[0]=struct.unpack_from('<H',mem,(r[4]&0xffffff)+r[5])[0]|0xabcd0000
   elif t==0x8c646bbc:
    struct.pack_into('<2H',mem,(r[4]&0xffffff)+4,width,height);r[0]=0xffffffff if decode_error else 0
   elif t==0x8c10ca5c:r[0]=r[0]//r[1]
   else:r[0]=0
   return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0],1,native));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr,0,mem));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[5]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6608a4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  assert r[0]==(0xffffff01 if decode_error else 0)
  decoded=[struct.unpack_from('<H',packetdata,k)[0] for k in [0,2,4,6,8,10,12,16,18,20]]
  offsets=[0,4,8,12,16,20,24,28,32,52]
  assert [struct.unpack_from('<I',native,0xa02000+k)[0] for k in offsets]==decoded
  assert [struct.unpack_from('<I',native,0xa02000+k)[0] for k in [36,40,44,56]]==[packetdata[14],packetdata[15],511,128]
  if not decode_error:assert struct.unpack_from('<4I',native,0xa0203c)==(height,width,height//512,width//512)
  assert sum(t[0]==0x8ca05000 for t in actual)==10
  count+=1
 report=dict(cases_passed=count,instruction_count=216,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_backend_request.c').read_bytes()).hexdigest(),scope='Successful packet field decoding and metadata error path across eight channels, dimensions and FPU banks. Full retail state/call comparison with independent field checks. Packet reader, metadata, division and device services injected; suspension unverified.')
 (out/'analysis/input_backend_packet_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'backend packet comparisons passed')
if __name__=='__main__':main()
