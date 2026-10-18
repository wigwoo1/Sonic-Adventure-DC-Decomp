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
video_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_video_state_initialize.dll'))
video=video_lib.sa_engine_video_state_initialize;video.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
identity=video_lib.sa_engine_video_state_identity;identity.argtypes=abi;functions[0x8c6513ee]=identity
manager=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_memory_manager_initialize.dll')).sa_engine_memory_manager_initialize;manager.argtypes=abi;functions[0x8c663140]=manager
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_memory_manager_reset.dll')).sa_engine_memory_manager_reset;reset.argtypes=abi;functions[0x8c663080]=reset
descriptor_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_descriptor_lifecycle.dll'))
for address,name in [(0x8c664ac0,'sa_engine_descriptor_allocate'),(0x8c664aec,'sa_engine_descriptor_release')]:
 helper=getattr(descriptor_lib,name);helper.argtypes=abi;functions[address]=helper
graphics_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_graphics_register_io.dll'))
graphics={}
for address,name in [(0x8c653440,'sa_engine_graphics_initialize_leaf'),(0x8c653444,'sa_engine_graphics_register_write'),(0x8c653476,'sa_engine_graphics_register_read')]:
 helper=getattr(graphics_lib,name);helper.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];graphics[address]=helper
graphics_base=struct.unpack_from('<I',binary,0x6534c0-0x10000)[0]
targets=[0x8c65253e,0x8c650aa0,0x8c666706]
cleared=[0x8a2f40,0x8a2ffc,0x8a30c4,0x8a30c8,0x8a328c,0x8a3238]+list(range(0x8a323c,0x8a3260,4))+list(range(0x8a3268,0x8a327c,4))
cases=0
for mode,result,bank,vbr,initial_sr in itertools.product([0,0xffffffff],[0],[0],[0x8c000000],[0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c004,0x8cb08000)
 for address in cleared:struct.pack_into('<I',ram,address,0xcccccccc)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 mmio={a:0x12345678 for a in [0xa05f6930,0xa05f6938,0xa05f6900,0xa05f6908]};mmio[0xa05f689c]=0;mmio.update({graphics_base+offset:0 for offset in range(0,0x1000,4)});mmio[graphics_base]=0x17fd11db;mmio[graphics_base+4]=0x11;registers=dict(mmio);writes=[];reads=[];counts=[0,0]
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('unexpected read',a,n));return 0
  reads.append(a);return registers[a]
 @Write
 def write(ctx,a,value,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=value;writes.append((a,value))
 io=IO(None,read,write)
 def service(target,m,r,events,which):
  if target==0x8c65253e:
   index=counts[which]
   assert r[4]==0xa5000000+index*4 and r[6]==4
   offset=r[5]&0xffffff;assert bytes(m[offset:offset+4])==bytes(4)
   counts[which]+=1;r[0]=0;return
  events.append((target,tuple(r[i] for i in range(4,8))))
  if target==0x8c653476:r[0]=0x17fd11db if r[4]==0 else 0x11
  elif target in targets:r[0]=result
  else:raise AssertionError(hex(target))
 hooks={}
 for target in targets:
  def hook(m,r,sr,target=target):service(target,m,r,expected,0);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target in graphics:return graphics[target](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target==0x8c6656e2:return video(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target==0x8c653640:return registrar(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),vbr,ctypes.byref(io),None,call)
  if target in functions:return functions[target](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target not in hooks:errors.append(target);return -9
  service(target,m,r,actual,1);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[15]=0x8cf00000;s.pc=0x8c65262c;s.pr=0x8c000100;s.sr=initial_sr
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special,mmio=mmio,vbr=vbr)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected and macl.value==special['macl'] and registers==mmio
 valid=mode in [0,1,2]
 assert counts==([0x200000,0x200000] if valid else [0,0])
 if valid:
  assert reads==[graphics_base,graphics_base+4,0xa05f689c]
  assert struct.unpack_from('<I',native,0x8a2d58)[0]==6
  assert struct.unpack_from('<I',native,0x8a3060)[0]==0x800408
 else:assert not actual
 assert r[0]==(0 if valid else 2)
 assert r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<I',native,0x67c004)[0]==0x8cb08000
 if valid:
  assert struct.unpack_from('<H',native,0x8a83e4)[0]==1
  assert struct.unpack_from('<H',native,0x8a83fc)[0]==1
  assert all(struct.unpack_from('<H',native,0x8a83e4+24*i)[0]==0 for i in range(2,4096))
  assert struct.unpack_from('<I',native,0x8a2d6c)[0]==0x800000
  assert struct.unpack_from('<5I',native,0x67c2cc)==(0x400000,0,0x400000,0x400000,0x400000)
  assert struct.unpack_from('<II',native,0x67c2e8)==(0x8c8a83e4,0x8c8a83e4)
  assert struct.unpack_from('<II',native,0x67c2fc)==(0x8c8a83fc,0x8c8a83fc)
  assert struct.unpack_from('<II',native,0x8a83f0)==(0,0x400000)
  assert struct.unpack_from('<II',native,0x8a8408)==(0x400000,0x400000)
  configured=[(address-graphics_base,value) for address,value in writes if graphics_base<=address<graphics_base+0x1000]
  assert configured[-134:]==[(8,1),(8,0),(0xe4,0),(0xb8,0xff07),(0xb4,0x7f7f7f),(0xb0,0x7f7f7f)]+[(0x200+4*i,((0xff-i)<<8)|(0xfe-i)) for i in range(128)]
  for address,value in [(0x8a3194,0x800000),(0x8a319c,0x600000),(0x8a31a0,0x600000),(0x8a31b8,0x400000),(0x8a31d8,0),(0x8a31f8,1)]:assert struct.unpack_from('<I',native,address)[0]==value
  assert struct.unpack_from('<I',native,(vbr&0xffffff)+0x224)[0]==0x8c652210
  assert [(address,value) for address,value in writes if not graphics_base<=address<graphics_base+0x1000]==[(0xa05f6930,0),(0xa05f6938,0),(0xa05f6900,0xffffffff),(0xa05f6908,0xffffffff),(0xa05f6930,0x2807ec),(0xa05f6938,14)]
  assert all(struct.unpack_from('<I',native,address)[0]==0 for address in cleared)
  assert struct.unpack_from('<4I',native,0x8a31a4)==(0x100000,0,0,0)
  assert struct.unpack_from('<I',native,0x8a2d74)[0]==mode
 else:assert not writes
 cases+=1
report={'cases_passed':cases,'entry':'0x8c65262c','video_clear_word_count':0x200000,'scope':'Connected native mode65262c, engine6510a4, option/string selection, device651384, video6656e2/forwarder6513ee, layout6666c0, callback state651520/registrar653640 and table665a00. Mode0 with supported hardware ID17fd11db revision17 bus0 and invalid mode-1; one VBR/SR/FPU configuration. Full retail RAM/CPU/FPU/SR/PR/MACL/MMIO/event equality. Independent2097152 zero-word clear requests, vector, interrupt writes, table arguments, layout values and object-state clearing. Native memory manager663140 and reset663080 connected with original option selection; independent8MiB capacity, two4MiB descriptors and linked heads/tails. Native descriptor allocator664ac0 connected; first two pool entries allocated and remaining4094 flags clear. Native no-op leaf653440 and register read/write653476/653444 connected. Only rendering-layout666706, VRAM clear65253e and service650aa0 modeled. Hardware MMIO is a register dictionary. Engine context placement synthetic; actual boot order and rendering unverified.'}
(root/'analysis/engine_mode_initialize_register_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'engine mode initializer comparisons passed')

