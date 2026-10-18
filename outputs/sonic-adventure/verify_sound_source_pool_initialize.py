import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();stem='sound_source_pool_initialize';address=0x8c65e39c
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=abi
initializing=stem.endswith('initialize');cases=0;pool=0x8a65b8
for pattern,selector,bank,stack in itertools.product([0,1,127,128,255,0x55],[0,1,2,0x7fffffff,0x80000000,0xfffffffe,0xffffffff] if initializing else [0,0x8ca20000],[0,1],[0,0x8ca40000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[pool-8:pool+3080]=bytes([pattern])*3088;ram[0xa1fff8:0xa20038]=bytes([pattern])*64
 struct.pack_into('<I',ram,0x67c264,selector);struct.pack_into('<I',ram,0xf00000,stack)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=selector if not initializing else 0x8ca20000;s.r[15]=0x8cf00000;s.pc=address;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);calls=[]
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a==0x8c10d31c,hex(a)
  calls.append(tuple(regs[i] for i in range(4,7)))
  return fill(m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 if initializing:
  cleared=ctypes.c_int32(selector).value<=0
  assert struct.unpack_from('<I',native,0x67c264)[0]==((selector+1)&0xffffffff if cleared else selector)
  assert native[pool:pool+3072]==(bytes(3072) if cleared else bytes([pattern])*3072)
  assert native[0xa1fff8:0xa20038]==bytes([pattern])*64
  assert calls==([(0x8c8a65b8,0,3072)] if cleared else [])
 else:
  assert native[0xa20000:0xa20030]==(bytes(48) if selector else bytes([pattern])*48)
  assert native[0xa1fff8:0xa20000]==bytes([pattern])*8 and native[0xa20030:0xa20038]==bytes([pattern])*8
  assert native[pool:pool+3072]==bytes([pattern])*3072
  assert calls==([(0x8ca20000,0,48)] if selector else [])
 assert native[pool-8:pool]==bytes([pattern])*8 and native[pool+3072:pool+3080]==bytes([pattern])*8 and r[15]==0x8cf00000
 cases+=1
scope=('168 full retail RAM/CPU/FPU/SR/PR comparisons of guarded source-pool initializer65e39c..65e3b8. Seven initialization counter patterns including negative/extreme values,six pool byte patterns,both banks,two stack fixtures. Independent signed-positive counter no-op; otherwise counter increments modulo32bits and entire3072-byte pool cleared. Native byte fill;8-byte guards and unrelated object preserved.' if initializing else '48 full retail RAM/CPU/FPU/SR/PR comparisons of source destruction65e42c..65e44a. Null/non-null pointer,six byte patterns,both banks,two stack fixtures. Independent entire48-byte object cleared only for non-null,8-byte guards and3072-byte pool preserved. Native byte fill.')+' No modeled calls. Complete construction/destruction/reuse integration and fullgame remain unverified.'
(p/f'analysis/{stem}_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,stem,'comparisons passed')
