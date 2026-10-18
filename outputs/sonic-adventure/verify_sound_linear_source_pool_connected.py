import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for a,stem in [(0x8c65e0c0,'sound_linear_source_pool_initialize'),(0x8c65e0ea,'sound_linear_source_create'),(0x8c65e178,'sound_linear_source_reset'),(0x8c65e150,'sound_linear_source_destroy'),(0x8c10d31c,'byte_fill')]:
 helpers[a]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[a].argtypes=abi
pool=0x8a6138;cases=0;operations=0
for pattern,size,bank in itertools.product([0,1,127,128,255,0x55],[0,64,1024],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[pool-8:pool+1160]=bytes([pattern])*1168;struct.pack_into('<I',ram,0x67c22c,0)
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
  assert native[pool-8:pool]==bytes([pattern])*8 and native[pool+1152:pool+1160]==bytes([pattern])*8 and r[15]==0x8cf00000
  operations+=1
 execute(0x8c65e0c0)
 assert native[pool:pool+1152]==bytes(1152) and struct.unpack_from('<I',native,0x67c22c)[0]==1
 wanted=bytearray(1152)
 for cycle in range(2):
  for slot in range(33):
   buffer=0x8ca40000+slot*2048;selected=0x8c000000|pool+slot*36
   execute(0x8c65e0ea,[buffer,size,16])
   if slot<32:
    values=[0x8c67c1fc,1,0x8c672424,size,0,buffer,size,0x8c65e0bc,selected]
    struct.pack_into('<9I',wanted,slot*36,*values)
   assert native[pool:pool+1152]==wanted and r[0]==(selected if slot<32 else 0)
  before=bytes(native);execute(0x8c65e0c0)
  assert native==before and struct.unpack_from('<I',native,0x67c22c)[0]==1
  for slot in list(range(1,32,2))+list(range(0,32,2)):
   execute(0x8c65e150,[0x8c000000|pool+slot*36]);wanted[slot*36:(slot+1)*36]=bytes(36)
   assert native[pool:pool+1152]==wanted
  assert native[pool:pool+1152]==bytes(1152)
 cases+=1
(p/'analysis/sound_linear_source_pool_connected_verification.json').write_text(json.dumps(dict(cases_passed=cases,native_operations_compared=operations,scope='36 persistent native linear-source pool lifecycles:six initial byte patterns,lengths0/64/1024,both banks. Guarded pool initialization counter0 to1;two cycles of32 native constructors plus exhaustion/null,idempotent initializer and32 native destructions in odd/even order. No RAM/CPU/FPU resets between calls. Full retail RAM/CPU/FPU/SR/PR comparison after every public call;independent entire1152-byte pool,all9 constructed words,first-free selection,returned pointers,occupied slots preserved,full clearing,reallocation,8-byte guards,counter and stack. Native reset and byte fill,no modeled calls. Reservations separately verified;retail default callback,game caller integration/fullgame remain unverified.'),indent=2)+'\n')
print(cases,'native linear source pool lifecycles;',operations,'operations passed')
