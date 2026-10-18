import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();stem='SETTER_STEM';entry=SETTER_ADDRESS
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/signed_division.dll')).sa_signed_division;division.argtypes=abi
cases=0
for present,value,count,bank in itertools.product([0,1],[1,2,4,8,16,24,32,0xffffffff],[0,1,7,8,0x7fffffff,0x80000000,0xffffffff],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa08000:0xa08080]=bytes([0x55])*128
 struct.pack_into('<I',ram,0xa08008,16);struct.pack_into('<I',ram,0xa08030,count)
 before=bytes(ram[0xa08000:0xa08080]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[]
 @Call
 def call(ctx,a,m,fp,srp,r,prp):
  calls.append(a)
  if a==0x8c10ca5c:return division(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c667b7a and r[4]==0x8c67217c;r[0]=0x12345678;return 0
 def error_hook(m,r,sr):
  events.append(0x8c667b7a);assert r[4]==0x8c67217c;r[0]=0x12345678;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:6]=[0x8ca08000 if present else 0,value];s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,{0x8c667b7a:error_hook},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 wanted=bytearray(before)
 if present:
  if stem.endswith('field8_set'):
   product=ctypes.c_int32((count*16)&0xffffffff).value;n=(abs(product)//8)*(-1 if product<0 else 1);d=ctypes.c_int32(value).value
   result=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1)
   struct.pack_into('<I',wanted,8,value);struct.pack_into('<I',wanted,0x30,(result<<3)&0xffffffff)
   assert calls==[0x8c10ca5c]
  else:struct.pack_into('<I',wanted,0xc,value);assert not calls
 else:assert calls==events==[0x8c667b7a]
 assert native[0xa08000:0xa08080]==wanted and r[15]==0x8cf00000
 cases+=1
scope='224 full retail RAM/CPU/FPU/SR/PR/MACL comparisons of subordinate setter '+hex(entry)+'. Null/present handles,eight nonzero full-word values,seven count patterns,both FPU banks. Independent128-byte object/guard window. Field8 setter rescales field30 via low32-bit multiply by old field8=16,signed truncation /8,native signed division by new field8,and low32-bit shift left3. Fieldc setter stores fullR5 in RTS delay slot. Null path forwards error string8c67217c to modeled reporter667b7a;native reporter/game callers/audio hardware unverified. Non-null division executes natively. Zero divisor not covered;field semantic names await caller recovery.'
(p/f'analysis/{stem}_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,stem,'comparisons passed')
