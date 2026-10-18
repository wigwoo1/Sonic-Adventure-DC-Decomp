import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_power_two_factor.dll')).sa_sound_driver_power_two_factor
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for value,bank,sr0 in itertools.product([0,1,2,3,4,6,16,64,128,192,256,1024,0x80000000,0xffffffff],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa20000:0xa2000c]=bytes([0xa5])*12
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10cbb8;calls.append((a,r[0],r[1]));r[0]=r[1]%r[0];return 0
 def hook(m,r,sr):events.append((0x8c10cbb8,r[0],r[1]));r[0]=r[1]%r[0];return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[5]=value;s.r[6]=0x8ca20004;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6687bc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8c10cbb8:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and calls==events
 expected=min(value&-value,128) if value else 0
 assert struct.unpack_from('<I',native,0xa20004)[0]==expected
 assert native[0xa20000:0xa20004]==bytes([0xa5])*4 and native[0xa20008:0xa2000c]==bytes([0xa5])*4
 assert all(a==0x8c10cbb8 and numerator==value and divisor&(divisor-1)==0 for a,divisor,numerator in calls)
 cases+=1
(out/'analysis/sound_driver_power_two_factor_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native28-instruction factor helper6687bc. Zero/odd/even/powers-of-two/cap/high-bit values,banks/SR. Full retail state/traces match with modeled unsigned remainder. Independent lowest-set-bit factor capped128,zero special case,neighbor preservation and divisor arguments checked. Remainder service modeled; full game unverified.'),indent=2)+'\n')
print(cases,'power two factor comparisons passed')
