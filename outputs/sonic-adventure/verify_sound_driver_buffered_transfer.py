import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_buffered_transfer.dll')).sa_sound_driver_buffered_transfer;fn.argtypes=abi
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=abi
copy=0x8c669014;transfer=0x8c6684c4;cases=0
for index,start,count,bank,sr0 in itertools.product([0,1],[-1,0,63],[0,1,63,64,128],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 handle=0x8ca08000;buffer=0x8ca30000;source=0x8ca20000
 struct.pack_into('<2I',ram,0xa08030,source,source)
 struct.pack_into('<I',ram,0xf00000,buffer)
 ram[0xa20000:0xa24000]=bytes((i*17+i//256)&255 for i in range(16384))
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in (copy,transfer)
  stack=sum(int(m[(r[15]&0xffffff)+j])<<(j*8) for j in range(4)) if a==transfer else None
  calls.append((a,r[4],r[5],r[6],r[7],stack))
  if a==copy:
   data=bytes(m[(r[4]&0xffffff)+j] for j in range(r[6]))
   for j,v in enumerate(data):m[(r[5]&0xffffff)+j]=v
  r[0]=123;return 0
 def hook(a):
  def run(m,r,sr):
   stack=struct.unpack_from('<I',m,r[15]&0xffffff)[0] if a==transfer else None
   events.append((a,r[4],r[5],r[6],r[7],stack))
   if a==copy:m[r[5]&0xffffff:(r[5]&0xffffff)+r[6]]=m[r[4]&0xffffff:(r[4]&0xffffff)+r[6]]
   r[0]=123;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=index;s.r[6]=start&0xffffffff;s.r[7]=count;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6687f4
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={a:hook(a) for a in (copy,transfer)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and calls==events
 expected=[];active=buffer;other=buffer+256
 def signed_mod64(v):return (abs(v)%64)*(-1 if v<0 else 1)
 for outer in range(count):
  ring=signed_mod64(start+outer)
  expected.append((copy,source+ring*256,active,256))
  for inner in range(64//count):
   next_ring=signed_mod64(ring-start+64)
   dest=source+next_ring*256
   expected.append((copy,dest,other,256))
   expected.append((transfer,handle,index,dest,active,256))
   ring=next_ring;active,other=other,active
 assert len(calls)==len(expected),(index,start,count,len(calls),len(expected),calls[:3])
 for actual,wanted in zip(calls,expected):assert actual[:len(wanted)]==wanted,(count,start,actual,wanted)
 cases+=1
(out/'analysis/sound_driver_buffered_transfer_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native137-instruction buffered transfer6687f4 with native signed division. Channels0/1,start-1/0/63,count0/1/63/64/128,banks/SR. Full retail RAM/CPU/FPU/SR/PR/traces match. Independent ring offsets,256-byte copy arguments,buffer alternation,transfer arguments/fifth stack length256 and call counts checked. Copy and lower transfer modeled returning123; purpose/physical transfer/full game unverified.'),indent=2)+'\n')
print(cases,'buffered transfer comparisons passed')
