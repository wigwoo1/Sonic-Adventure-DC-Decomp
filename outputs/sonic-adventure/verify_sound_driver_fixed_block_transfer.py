import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_fixed_block_transfer.dll')).sa_sound_driver_fixed_block_transfer
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def word(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
configure=word(0x8c6686e0);consume=word(0x8c6686e8);error=word(0x8c6686c0)
cases=0
for selector,config_result,transfer_result,bank,sr0 in itertools.product([-1,0,1,2,255,256],[0,123],[0,456],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 handle=0x8ca08000;mode=0x10203040;address=0x8ca20000
 for channel in [-1,0,1,2]:struct.pack_into('<I',ram,0xa08020+channel*4,0x8ca10000+64*(channel+1))
 target=0x8ca10000+64*(ctypes.c_int8(selector).value+1)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];stacks=[];refstacks=[]
 def result(a):
  assert a in (configure,consume,error)
  return config_result if a==configure else transfer_result if a==consume else 777
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  calls.append((a,r[4],r[5],r[6],r[7]))
  if a==configure:stacks.append(sum(int(m[(r[15]&0xffffff)+j])<<(j*8) for j in range(4)))
  r[0]=result(a);return 0
 def hook(a):
  def run(m,r,sr):
   events.append((a,r[4],r[5],r[6],r[7]))
   if a==configure:refstacks.append(struct.unpack_from('<I',m,r[15]&0xffffff)[0])
   r[0]=result(a);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=selector&0xffffffff;s.r[6]=mode;s.r[7]=address;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66854e
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (configure,consume,error)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert calls==events and stacks==refstacks==[0]
 assert calls[0]==(configure,target,address,32,0)
 expected=[configure,error] if config_result else [configure,consume]+([error] if transfer_result else [])
 assert [x[0] for x in calls]==expected
 if not config_result:assert calls[1][0:3]==(consume,target,mode)
 if config_result or transfer_result:assert calls[-1][1]==word(0x8c6686e4 if config_result else 0x8c6686ec)
 assert r[0]==(config_result or transfer_result)
 cases+=1
(out/'analysis/sound_driver_fixed_block_transfer_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native55-instruction fixed32-byte transfer66854e. Signed byte selector/truncation,banks/SR,configuration/transfer returns. Full retail RAM/CPU/FPU/SR/PR/MACL and traces match. Independent selected object,address,fixed length32,zero callback and stack argument,transfer mode,conditional error messages and final return checked. Lower configuration/consumer/diagnostics modeled; actual transfer/audio unverified.'),indent=2)+'\n')
print(cases,'fixed block transfer comparisons passed')
