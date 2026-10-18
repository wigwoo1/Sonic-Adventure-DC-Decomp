import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_transfer.dll'))
fn=lib.sa_sound_driver_transfer;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
cases=0
for bank in [0,0x200000]:
 rejection='success';result=0;mask=6;handle=0x8cb20000
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0xb10008,0x8cb00000)
 struct.pack_into('<I',ram,0xb00000,0 if rejection=='magic' else 0x56524453)
 ram[0xb00004]=0 if rejection=='version' else 1;ram[0xb00005]=2 if rejection=='type' else 0
 struct.pack_into('<I',ram,0x67379c,0)
 struct.pack_into('<I',ram,0xb00008,64)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 targets=[struct.unpack_from('<I',binary,a-0x10000)[0] for a in [0x643f34,0x643f38,0x643f40,0x643f44,0x643f48,0x643f4c,0x643f58,0x643f5c,0x643f60]]
 def service(t,m,r,events):
  events.append((t,tuple(r[i] for i in range(4,8))))
  if t==targets[2]:
   a=r[4]&0xffffff
   for i in range(4):m[a+i]=(handle>>(8*i))&255
  r[0]=0
 hooks={}
 for t in targets:
  def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t not in targets:errors.append(t);return -9
  service(t,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c643e6c;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 s.r[4]=0 if rejection=='null' else 0x8cb10000
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks=hooks,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==((0x8c643e6c,result) if result else (0xabcdef01,0x12345678))
 assert [e[0] for e in actual]==targets
 assert actual[2][1][0]==0x8c67379c
 assert actual[3][1]==(handle,0x8cb00020,64,0xffffffff)
 assert actual[4][1][:2]==(handle,0)
 assert actual[7][1][0]==0x8cb10000 and actual[8][1][0]==handle
 assert struct.unpack_from('<I',native,0x67c1d4)[0]==1
 assert r[12]==0x2234000c and r[13]==0x2234000d
 cases+=1
report={'cases_passed':cases,'instructions_recovered':91,'scope':'Full original-instruction RAM/CPU/SR/PR/FPU/service-event equality on valid SDR V driver header, payload64, both FPU banks. Original2097152-iteration delay executed without patching. Independent complete nine-call order, object configure payload/length/callback, queue args, enable flag, callback/release arguments, preserved stack/R12/R13/R14 and zero result. All subordinate routines modeled; hardware/actual driver execution unverified.'}
(out/'analysis/sound_driver_transfer_success_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver transfer success comparisons passed')
