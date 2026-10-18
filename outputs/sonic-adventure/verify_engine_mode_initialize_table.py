import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_mode_initialize.dll')).sa_engine_mode_initialize;fn.argtypes=abi
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll'))
functions={}
for address,name in [(0x8c6510a4,'sa_engine_context_initialize_native'),(0x8c665a80,'sa_engine_option_select'),(0x8c10d3c8,'sa_string_compare_wrapper'),(0x8c10db1c,'sa_string_compare')]:
 helper=getattr(lib,name);helper.argtypes=abi;functions[address]=helper
device=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_device_initialize.dll')).sa_engine_device_initialize;device.argtypes=abi;functions[0x8c651384]=device
callbacks=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_callback_state_initialize.dll')).sa_engine_callback_state_initialize;callbacks.argtypes=abi;functions[0x8c651520]=callbacks
registrar=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_callback_register.dll')).sa_engine_callback_register
registrar.argtypes=abi[:7]+[ctypes.c_uint32,ctypes.POINTER(IO)]+abi[7:]
layout=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_memory_layout_initialize.dll')).sa_engine_memory_layout_initialize;layout.argtypes=abi;functions[0x8c6666c0]=layout
table=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_register_table_initialize.dll')).sa_engine_register_table_initialize;table.argtypes=abi;functions[0x8c665a00]=table
targets=[0x8c653440,0x8c6656e2,0x8c653444,0x8c650aa0,0x8c663140,0x8c666706]
cleared=[0x8a2f40,0x8a2ffc,0x8a30c4,0x8a30c8,0x8a328c,0x8a3238]+list(range(0x8a323c,0x8a3260,4))+list(range(0x8a3268,0x8a327c,4))
cases=0
for mode,result,bank,vbr,initial_sr in itertools.product([0xffffffff,0,1,2,3,7],[0,0x12345678],[0,0x200000],[0x8c000000,0x8ca08000],[0x70000063,0x700000f3]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c004,0x8cb08000)
 for address in cleared:struct.pack_into('<I',ram,address,0xcccccccc)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 mmio={a:0x12345678 for a in [0xa05f6930,0xa05f6938,0xa05f6900,0xa05f6908]};registers=dict(mmio);writes=[]
 @Read
 def read(ctx,a,n):errors.append(('unexpected read',a,n));return 0
 @Write
 def write(ctx,a,value,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=value;writes.append((a,value))
 io=IO(None,read,write)
 def service(target,r,events):
  events.append((target,tuple(r[i] for i in range(4,8))))
  if target in targets:r[0]=result
  else:raise AssertionError(hex(target))
 hooks={}
 for target in targets:
  def hook(m,r,sr,target=target):service(target,r,expected);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target==0x8c653640:return registrar(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),vbr,ctypes.byref(io),None,call)
  if target in functions:return functions[target](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target not in hooks:errors.append(target);return -9
  service(target,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[15]=0x8cf00000;s.pc=0x8c65262c;s.pr=0x8c000100;s.sr=initial_sr
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special,mmio=mmio,vbr=vbr)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected and macl.value==special['macl'] and registers==mmio
 valid=mode in [0,1,2]
 assert [e[0] for e in actual]==([0x8c653440,0x8c6656e2,0x8c653444,0x8c653444,0x8c650aa0,0x8c663140]+[0x8c653444]*131+[0x8c666706] if valid else [])
 assert r[0]==(0 if valid else 2)
 assert r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<I',native,0x67c004)[0]==0x8cb08000
 if valid:
  assert actual[0][1][0]==mode
  configured=[event[1][:2] for event in actual if event[0]==0x8c653444]
  assert configured==[(8,1),(8,0),(0xb8,0xff07),(0xb4,0x7f7f7f),(0xb0,0x7f7f7f)]+[(0x200+4*i,((0xff-i)<<8)|(0xfe-i)) for i in range(128)]
  for address,value in [(0x8a3194,0x800000),(0x8a319c,0x600000),(0x8a31a0,0x600000),(0x8a31b8,0x400000),(0x8a31d8,0),(0x8a31f8,1)]:assert struct.unpack_from('<I',native,address)[0]==value
  assert struct.unpack_from('<I',native,(vbr&0xffffff)+0x224)[0]==0x8c652210
  assert writes==[(0xa05f6930,0),(0xa05f6938,0),(0xa05f6900,0xffffffff),(0xa05f6908,0xffffffff),(0xa05f6930,0x2807ec),(0xa05f6938,14)]
  assert all(struct.unpack_from('<I',native,address)[0]==0 for address in cleared)
  assert struct.unpack_from('<4I',native,0x8a31a4)==(0x100000,0,0,0)
  assert struct.unpack_from('<I',native,0x8a2d74)[0]==mode
 else:assert not writes
 cases+=1
report={'cases_passed':cases,'instructions_recovered':28,'new_instructions_recovered':42,'entry':'0x8c65262c','scope':'Retail CPU/RAM/FPU/SR/PR comparison, mode0/1/2 and invalid signed modes. Independent unconditional engine-init ordering, valid option/device dispatch, stored selection, return0/2 and callee-saved/stack checks. Native engine initializer, parameter setup, interrupt defaults, byte fill, option selector and string comparison connected; Native device initializer651384 and callback-state initializer651520 connected; Native callback registrar653640 connected with explicit VBR/MMIO; Native memory-layout initializer6666c0 connected; Native register-table initializer665a00 connected; four device services plus register configuration653444 and service650aa0 modeled. Independent three fixed and128 generated table-configuration arguments checked. Independent vector install, six ordered MMIO writes and no MMIO writes for invalid modes checked; two VBR bases and interrupt masks. Engine context placement synthetic; boot order not established. Independent callback target652210 and zeroed state fields checked from nonzero canaries.'}
(root/'analysis/engine_mode_initialize_table_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'engine mode initializer comparisons passed')

