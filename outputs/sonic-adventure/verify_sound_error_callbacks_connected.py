import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,stem in [(0x8c667b70,'sound_error_callback_set'),(0x8c667b7a,'sound_error_report'),(0x8c65e16e,'sound_linear_source_identity_get')]:
 helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[address].argtypes=abi
cases=0
for callback,user,message,bank in itertools.product([0,0x8c65e16e],[0x8ca10000+i*16 for i in range(5)],[0,0x8c67217c,0xffffffff,0x12345678],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10080]=bytes([0x55])*128
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);actual=[]
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a==callback==0x8c65e16e and (regs[4],regs[5])==(user,message)
  actual.append((a,regs[4],regs[5]))
  return helpers[a](m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 for entry,args in [(0x8c667b70,[callback,user]),(0x8c667b7a,[message])]:
  for i,v in enumerate(args,4):s.r[i]=v;r[i]=v
  s.pc=entry;reference(ram,s,{},fpu=f,special={'macl':0})
  assert helpers[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<2I',native,0x67c37c)==(callback,user) and native[0xa10000:0xa10080]==bytes([0x55])*128 and r[15]==0x8cf00000
 assert actual==([(callback,user,message)] if callback else [])
 cases+=1
scope='80 persistent native error callback registration/report sequences,both banks,null/controlled identity-getter callback,five user-data objects,four message words. Full retail RAM/CPU/FPU/SR/PR after both public operations without state resets. Independent globals67c37c=callback/67c380=user data preserved by report,user object bytes preserved,exact callback(user,message) forwarding/absence,stack restored. Controlled callback executes natively,no modeled calls. Application callback behavior and sound setter error integration/fullgame unverified.'
for stem in ['sound_error_callback_set','sound_error_report']:
 (p/f'analysis/{stem}_verification.json').write_text(json.dumps(dict(cases_passed=cases,native_operations_compared=cases*2,scope=scope),indent=2)+'\n')
print(cases,'native sound error sequences;',cases*2,'operations passed')
