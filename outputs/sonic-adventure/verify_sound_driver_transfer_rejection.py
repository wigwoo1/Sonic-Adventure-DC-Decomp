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
for rejection,handle,bank in itertools.product(['null','magic','version','type'],[0,0x8cb20000],[0,0x200000]):
 result={'null':0x07000103,'magic':0x09000001,'version':0x0b000002,'type':0x0b000002}[rejection];mask=6
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0xb10008,0x8cb00000)
 struct.pack_into('<I',ram,0xb00000,0 if rejection=='magic' else 0x56524453)
 ram[0xb00004]=0 if rejection=='version' else 1;ram[0xb00005]=2 if rejection=='type' else 0
 struct.pack_into('<I',ram,0x67379c,handle)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def hook(m,r,sr):expected.append(r[4]);r[0]=0;return sr
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==struct.unpack_from('<I',binary,0x643f60-0x10000)[0]:actual.append(r[4]);r[0]=0;return 0
  if t==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c643e6c;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 s.r[4]=0 if rejection=='null' else 0x8cb10000
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks={struct.unpack_from('<I',binary,0x643f60-0x10000)[0]:hook},fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected==([handle] if handle else [])
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==((0x8c643e6c,result) if result else (0xabcdef01,0x12345678))
 cases+=1
report={'cases_passed':cases,'instructions_recovered':91,'scope':'Full original-instruction RAM/CPU/SR/PR/FPU equality on null object, bad signature, invalid version/type; both FPU banks and optional cleanup handle. Native error recorder; independent return/error globals, cleanup argument, preserved stack/R14. Success transfer path unverified; release modeled.'}
(out/'analysis/sound_driver_transfer_rejection_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver transfer rejection comparisons passed')
