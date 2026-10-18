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
graphics_state=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_graphics_state_initialize.dll')).sa_engine_graphics_state_initialize;graphics_state.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
render_layout=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_layout_initialize.dll')).sa_engine_render_layout_initialize;render_layout.argtypes=abi;functions[0x8c666706]=render_layout
buffer_size=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_size.dll')).sa_engine_render_buffer_size;buffer_size.argtypes=abi;functions[0x8c665e00]=buffer_size
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_setup.dll')).sa_engine_render_buffer_setup;setup.argtypes=abi;functions[0x8c665fa8]=setup
buffer_registers=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_registers.dll')).sa_engine_render_buffer_registers;buffer_registers.argtypes=abi;functions[0x8c665e3c]=buffer_registers
transfer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_transfer.dll')).sa_engine_render_buffer_transfer;transfer.argtypes=abi;functions[0x8c65347c]=transfer
word_copy=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_word_copy.dll')).sa_engine_word_copy;word_copy.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
header=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_header.dll')).sa_engine_render_buffer_header;header.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
targets=[0x8c65253e,0x8c10cb10,0x8c10cc80]
cleared=[0x8a2f40,0x8a2ffc,0x8a30c4,0x8a30c8,0x8a328c,0x8a3238]+list(range(0x8a323c,0x8a3260,4))+list(range(0x8a3268,0x8a327c,4))
cases=0
for width,height,index,transfer_mode,result,bank,vbr,initial_sr in itertools.product([1,2],[1,3],[0,1],[0,32],[0],[0,0x200000],[0x8c000000],[0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c004,0x8cb08000)
 for address in cleared:struct.pack_into('<I',ram,address,0xcccccccc)
 literal=lambda a:struct.unpack_from('<I',binary,a-0x8c010000)[0]
 struct.pack_into('<I',ram,literal(0x8c6663ac)&0xffffff,width)
 struct.pack_into('<I',ram,literal(0x8c665f14)&0xffffff,6)
 struct.pack_into('<I',ram,0xf00000,index)
 buffer_start=0x80+index*0x100
 struct.pack_into('<I',ram,(literal(0x8c6660c8)+32*index)&0xffffff,buffer_start)
 struct.pack_into('<I',ram,literal(0x8c6660cc)&0xffffff,transfer_mode)
 struct.pack_into('<I',ram,literal(0x8c6660d0)&0xffffff,1)
 header_start=0x800+index*0x100
 struct.pack_into('<2I',ram,(literal(0x8c6660d4)+32*index)&0xffffff,header_start,header_start+0x40)
 buffer_start=struct.unpack_from('<I',ram,(literal(0x8c6660c8)+32*index)&0xffffff)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 mmio={a:0x12345678 for a in [0xa05f6930,0xa05f6938,0xa05f6900,0xa05f6908]};mmio[0xa05f689c]=0;mmio.update({a:0x12345678 for a in [0xffa00020,0xffa00024,0xffa00028,0xffa0002c,0xffa00040,0xa05f6884,0xff000038,0xff00003c]});mmio.update({graphics_base+offset:0 for offset in range(0,0x1000,4)});mmio[graphics_base]=0x17fd11db;mmio[graphics_base+4]=0x11;mmio.update({0xa5000000+i:0xa5a5a5a5 for i in range(0,4096,4)});registers=dict(mmio);writes=[];reads=[];counts=[0,0]
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('unexpected read',a,n));return 0
  reads.append(a);return registers[a]
 @Write
 def write(ctx,a,value,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=value;writes.append((a,value))
 io=IO(None,read,write)
 vram=[bytearray([0xa5])*1024,bytearray([0xa5])*1024]
 def service(target,m,r,events,which):
  events.append((target,tuple(r[i] for i in range(4,8))))
  if target==0x8c653476:r[0]=0x17fd11db if r[4]==0 else 0x11
  elif target==0x8c65347c:
   offset=r[5]&0xffffff;events[-1]=events[-1]+(bytes(m[offset:offset+r[6]]).hex(),);r[0]=1
  elif target==0x8c665f42:r[0]=1
  elif target in targets:r[0]=result
  else:raise AssertionError(hex(target))
 def division(target,m,r,status,fpu):
  nested=State();nested.r[:]=[r[i] for i in range(16)];nested.pc=target;nested.pr=0x8c000100;nested.sr=status
  reference(m,nested,fpu=fpu)
  for i in range(16):r[i]=nested.r[i]
  return nested.sr
 hooks={}
 for target in targets:
  def hook(m,r,sr,target=target):
   if target==0x8c65253e:
    offset=r[5]&0xffffff;expected.append((target,tuple(r[i] for i in range(4,8)),bytes(m[offset:offset+r[6]]).hex()))
    nested=State();nested.r[:]=[r[i] for i in range(16)];nested.pc=target;nested.pr=0x8c000100;nested.sr=sr
    reference(m,nested,fpu=f,mmio=mmio)
    for i in range(16):r[i]=nested.r[i]
    return nested.sr
   if target in [0x8c10cb10,0x8c10cc80]:
    expected.append((target,tuple(r[:2])));return division(target,m,r,sr,f)
   service(target,m,r,expected,0);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target==0x8c665f42:return header(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target==0x8c65253e:
   offset=r[5]&0xffffff;actual.append((target,tuple(r[i] for i in range(4,8)),bytes(m[offset:offset+r[6]]).hex()))
   return word_copy(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target==0x8c650aa0:return graphics_state(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target in graphics:return graphics[target](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target==0x8c6656e2:return video(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if target==0x8c653640:return registrar(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),vbr,ctypes.byref(io),None,call)
  if target in functions:return functions[target](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target not in hooks:errors.append(target);return -9
  if target in [0x8c10cb10,0x8c10cc80]:
   actual.append((target,tuple(r[:2])))
   try:sr[0]=division(target,native,r,sr[0],nf)
   except Exception as exc:errors.append(('division callback',repr(exc)));return -9
   return 0
  service(target,m,r,actual,1);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=width;s.r[5]=height;s.r[6]=0;s.r[15]=0x8cf00000;s.pc=0x8c665fa8;s.pr=0x8c000100;s.sr=initial_sr
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special,mmio=mmio,vbr=vbr)
 assert setup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected and macl.value==special['macl'] and registers==mmio
 transfers=[event for event in actual if event[0]==0x8c65253e]
 expected_count=width*height*(4 if transfer_mode else 1)
 assert len(transfers)==expected_count,(width,height,index,transfer_mode,len(transfers))
 assert all(event[1][2]==24 and len(event[2])==48 for event in transfers)
 assert [event[1][0] for event in transfers]==[0xa5000000+buffer_start+24*i for i in range(expected_count)],(transfer_mode,[(hex(e[1][0]),e[1][2]) for e in transfers])
 if transfer_mode:
  header_address=0xa5000000+header_start+0x40+width*height*96
  assert registers[header_address]==((width*height*96//4+0x40//4+2)|literal(0x8c665ff4))
 else:header_address=None
 vram=[b''.join(values[0xa5000000+i].to_bytes(4,'little') for i in range(0,4096,4)) for values in [mmio,registers]]
 assert vram[0]==vram[1]
 allowed=set(range(buffer_start,buffer_start+24*expected_count))
 if header_address is not None:allowed.update(range(header_address-0xa5000000,header_address-0xa5000000+68))
 assert all(value==0xa5 for offset,value in enumerate(vram[1]) if offset not in allowed)
 assert r[15]==0x8cf00000
 cases+=1
report={'cases_passed':cases,'instructions_recovered':638,'scope':'Nonzero tile loop: widths1/2 heights1/3 buffer indices0/1 both FPU banks, distinct buffer base offsets. Native buffer register helper and register writer; full retail RAM/CPU/FPU/SR/PR/MACL/MMIO and helper events including 24-byte payload equality. Native transfer wrapper65347c connected; native copy65253e writes modeled VRAM; retail reference executes original copy instructions, native conditional header665f42 connected; transfer-mode flags0/32 tested with independent header count and destination checks; division routines interpreted from retail. Independent tile count, size, contiguous destinations and untouched VRAM tail checked; physical rendering unverified.'}
(root/'analysis/engine_render_buffer_setup_header_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'nonzero tile setup comparisons passed')

