import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for a,stem in [(0x8c65e39c,'sound_source_pool_initialize'),(0x8c65e3c6,'sound_source_create'),(0x8c65e454,'sound_source_reset'),(0x8c65e3b8,'sound_source_pool_counter_decrement'),(0x8c10d31c,'byte_fill')]:
 helpers[a]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[a].argtypes=abi
pool=0x8a65b8;cases=0;operations=0
for pattern,size,bank in itertools.product([0,1,127,128,255,0x55],[0,64,1024],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[pool-8:pool+3080]=bytes([pattern])*3088;struct.pack_into('<I',ram,0x67c264,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a in helpers,hex(a)
  return helpers[a](m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def execute(entry,args=()):
  global operations
  for i,v in enumerate(args,4):s.r[i]=v;r[i]=v
  s.pc=entry;reference(ram,s,{},fpu=f,special={'macl':0})
  assert helpers[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert native[pool-8:pool]==bytes([pattern])*8 and native[pool+3072:pool+3080]==bytes([pattern])*8 and r[15]==0x8cf00000
  operations+=1
 execute(0x8c65e39c)
 assert native[pool:pool+3072]==bytes(3072) and struct.unpack_from('<I',native,0x67c264)[0]==1
 wanted=bytearray(3072)
 for cycle in range(2):
  for slot in range(65):
   buffer=0x8ca40000+slot*2048;selected=0x8c000000|pool+slot*48
   execute(0x8c65e3c6,[buffer,size,16])
   if slot<64:
    values=[0x8c67c234,1,0x8c672434,0,size,0,0,buffer,size,16,0x8c65e398,selected]
    struct.pack_into('<12I',wanted,slot*48,*values)
   assert native[pool:pool+3072]==wanted and r[0]==(selected if slot<64 else 0)
  before=bytes(native);execute(0x8c65e39c)
  assert native==before and struct.unpack_from('<I',native,0x67c264)[0]==1
  live_pool=bytes(native[pool:pool+3072]);before_counter=bytes(native)
  execute(0x8c65e3b8)
  expected_counter=bytearray(before_counter);struct.pack_into('<I',expected_counter,0x67c264,0)
  assert native==expected_counter and native[pool:pool+3072]==live_pool and sr.value&1==0
  execute(0x8c65e39c)
  assert struct.unpack_from('<I',native,0x67c264)[0]==1 and native[pool:pool+3072]==bytes(3072)
  wanted=bytearray(3072)

 cases+=1
(p/'analysis/sound_source_pool_reinitialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,native_operations_compared=operations,scope='36 persistent native source-pool reinitialization sequences:six initial byte patterns,ring sizes0/64/1024,both banks. Initialize counter0 to1;two cycles each allocate all64 objects plus exhaustion check,verify positive-counter initializer preserves entire RAM,decrement1 to0 without touching live pool,then initialize to clear entire3072-byte pool and restore counter1. Second cycle reallocates cleared slots with exact original bindings and pointers. 4932 public native operations,each compared against retail full RAM/CPU/FPU/SR/PR,with independent all12 object fields,wholepool contents,counter transition,SR.T,guards,stack and preservation checks. Native source allocator/reset,bytefill,counterdecrement and initializer connected,no modeled calls. Reinitialization after decrement invalidates all old objects; this verifies raw retail behavior,not authorization or lifetime handling at game call sites. Game caller synchronization/fullgame remain unverified.'),indent=2)+'\n')
print(cases,'persistent source pool lifecycles;',operations,'native operations passed')
