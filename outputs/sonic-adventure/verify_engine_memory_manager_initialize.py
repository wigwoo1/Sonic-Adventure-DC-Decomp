import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_memory_manager_initialize.dll')).sa_engine_memory_manager_initialize;fn.argtypes=abi
targets=[0x8c665a80,0x8c653444,0x8c663080,0x8c664ac0]
cases=0
for mode,option,failure,bank in itertools.product([0,1,2],[0,1],[0,1,2],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c2c8,mode)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(target,r,events):
  events.append((target,tuple(r[i] for i in range(4,8))))
  assert target in targets
  if target==0x8c665a80:r[0]=option
  elif target==0x8c664ac0:
   index=sum(event[0]==target for event in events)
   r[0]=0 if index==failure else 0x8cb10000+(index-1)*0x100
  else:r[0]=0
 hooks={}
 for target in targets:
  def hook(m,r,sr,target=target):service(target,r,expected);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target not in hooks:errors.append(target);return -9
  service(target,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[15]=0x8cf00000;s.pc=0x8c663140;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 requested=1 if mode==1 else 2
 count=min(requested,failure) if failure else requested
 assert [event[0] for event in actual]==[0x8c665a80,0x8c653444,0x8c663080]+[0x8c664ac0]*count
 assert actual[1][1][:2]==(0xe4,0)
 assert r[0]==(3 if failure and failure<=requested else 0)
 capacity=0x1000000 if option==1 else 0x800000
 block=capacity if mode==1 else capacity//2
 offset=0 if mode==1 else block
 assert struct.unpack_from('<I',native,0x8a2d6c)[0]==capacity
 assert struct.unpack_from('<5I',native,0x67c2cc)==(block,0,block,block,offset)
 if failure!=1:
  assert struct.unpack_from('<II',native,0xb1000c)==(0,block)
  assert struct.unpack_from('<II',native,0x67c2e8)==(0x8cb10000,0x8cb10000)
 if requested==2 and failure!=1 and failure!=2:
  assert struct.unpack_from('<II',native,0xb1010c)==(block,offset)
  assert struct.unpack_from('<II',native,0x67c2fc)==(0x8cb10100,0x8cb10100)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':159,'entry':'0x8c663140','scope':'Full retail RAM/CPU/FPU/SR/PR equality for hardware flags0/1/2, selected options0/1, allocation success and first/second failure, both FPU banks. Independent capacities, block offsets, allocation calls, descriptor links and returns0/3 checked. Option selection, register write, manager reset and node allocation modeled; real allocation/boot unverified.'}
(root/'analysis/engine_memory_manager_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'memory manager initializer comparisons passed')
