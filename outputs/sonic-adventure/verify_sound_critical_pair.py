import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
functions=[]
for name in ['sound_critical_enter','sound_critical_exit']:
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{name}.dll')),'sa_'+name);fn.argtypes=abi;functions.append(fn)
cases=0
for depth,mask,bank in itertools.product([0,1,2,0x7ffffffe],[0,5,15],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<2I',ram,0x67c1e0,depth,9)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(*args):raise AssertionError('Unexpected service')
 for index,(address,fn) in enumerate(zip([0x8c65ad68,0x8c65ad8e],functions)):
  s.pc=address;reference(ram,s,fpu=f,special={'macl':0})
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<2I',native,0x67c1e0)==(depth+(1 if index==0 else 0),mask if depth==0 else 9)
  assert (sr.value>>4)&15==(15 if depth==0 and index==0 else mask)
 cases+=1
(p/'analysis/sound_critical_pair_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='24 paired native enter65ad68..65ad8e and exit65ad8e..65adb4 sequences; full retail RAM/CPU/FPU/SR/PR comparison on both calls. Depth0/1/2/7ffffffe,mask0/5/15,both banks,persistent CPU/RAM. Independent depth increment/decrement,saved mask first entry,interrupt mask15 only outer enter,original mask restored outer exit;nested pair preserves mask. Underflow/overflow/destructor integration/fullgame unverified.'),indent=2)+'\n')
print(cases,'critical section pairs passed')
