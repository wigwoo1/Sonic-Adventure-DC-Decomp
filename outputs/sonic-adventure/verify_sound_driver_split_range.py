import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_split_range.dll')).sa_sound_driver_split_range;fn.argtypes=abi
helpers={}
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_pool_queue_initialize.dll'))
for a,name in [(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd8e,'sa_engine_record_copy8_tail')]:
 f=getattr(lib,name);f.argtypes=abi;helpers[a]=f
cases=0
for base,length,limit,bank,sr0 in itertools.product([0,0xfffffff0],[0,1,256,0x7fffffff,0x80000000,0xffffffff],[0,1,128,0x7fffffff,0x80000000,0xffffffff],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0xa08000,base,length)
 struct.pack_into('<4I',ram,0xa10000,0xa5a5a5a5,0xdeadbeef,0x13579bdf,0x2468ace0)
 struct.pack_into('<4I',ram,0xa20000,0x11223344,0x55667788,0x99aabbcc,0xddeeff00)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in helpers,a
  return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=limit;s.r[6]=0x8ca10000;s.r[7]=0x8ca20000;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668af4
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 clamped=limit if ctypes.c_int32(length).value>ctypes.c_int32(limit).value else length
 assert struct.unpack_from('<4I',native,0xa10000)==(base,clamped,0x13579bdf,0x2468ace0)
 assert struct.unpack_from('<4I',native,0xa20000)==((base+clamped)&0xffffffff,(length-clamped)&0xffffffff,0x99aabbcc,0xddeeff00)
 assert struct.unpack_from('<2I',native,0xa08000)==(base,length)
 cases+=1
(out/'analysis/sound_driver_split_range_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native22-instruction descriptor split668af4 with native8-byte copy dispatcher/tail. Full retail RAM/CPU/FPU/SR/PR comparison. Independent signed clamp, wrapped end pointer, remainder length, source preservation and destination neighbor preservation. Bases0/FFFFFFF0; lengths and limits0/1/128or256/7FFFFFFF/80000000/FFFFFFFF, both FPU banks and SR masks. Distinct descriptors; alias behavior and full game unverified.'),indent=2)+'\n')
print(cases,'descriptor split comparisons passed')
