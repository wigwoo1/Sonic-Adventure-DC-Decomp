import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_buffer_read.dll')).sa_sound_buffer_read;fn.argtypes=abi
error_record=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll')).sa_engine_error_record;error_record.argtypes=abi
def word(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
lower=word(0x8c669078);error=word(0x8c669084);result_address=word(0x8c66907c)&0xffffff;cases=0
for source,length,misaligned,answer,bank,sr0 in itertools.product([0x1200,0x1234],[0,4,5,256],[False,True],[0,456],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 dest=0x8ca20000+int(misaligned);ram[0xa20000:0xa20104]=bytes([0xa5])*260;struct.pack_into('<I',ram,result_address,0xdeadbeef)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];stacks=[];refstacks=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==error:return error_record(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==lower;calls.append((a,r[4],r[5],r[6],r[7]))
  stacks.append(tuple(sum(int(m[(r[15]&0xffffff)+4*k+j])<<(8*j) for j in range(4)) for k in range(2)))
  r[0]=answer;return 0
 def hook(m,r,sr):
  events.append((lower,r[4],r[5],r[6],r[7]));refstacks.append(struct.unpack_from('<2I',m,r[15]&0xffffff));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=source;s.r[5]=dest;s.r[6]=length;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c669014
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={lower:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 rejected=misaligned or length%4!=0
 assert calls==events and stacks==refstacks
 if rejected:
  assert not calls and r[0]==0x7fffffff
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c669014,0x7fffffff)
  assert struct.unpack_from('<I',native,result_address)[0]==0xdeadbeef
 else:
  assert calls==[(lower,source+0xa0800000,dest,4,(length+3)//4)] and stacks==[(1,1)]
  assert r[0]==0 and struct.unpack_from('<I',native,result_address)[0]==answer
 assert native[0xa20000:0xa20104]==bytes([0xa5])*260
 cases+=1
(out/'analysis/sound_buffer_read_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native45-instruction G2 buffer read669014 and native error recorder. Source offsets,length0/4/5/256,destination alignment,lower returns,banks/SR. Full retail RAM/CPU/FPU/SR/PR matches. Independent G2 source bias,destination,stride4,word count,two stack flags1,stored lower result,zero success result,alignment error/source record and preserved destination checked. Lower G2 read modeled without payload writes; physical data transfer/full game unverified.'),indent=2)+'\n')
print(cases,'G2 buffer read wrapper comparisons passed')
