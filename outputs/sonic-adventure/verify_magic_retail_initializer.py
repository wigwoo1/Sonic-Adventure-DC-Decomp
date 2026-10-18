import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lower={}
for stem,entries in [('sound_stream_format_initialize',[(0x8c667596,'sa_sound_stream_format_initialize')]),('sound_magic_state_initialize',[(0x8c667488,'sa_sound_magic_state_initialize')]),('sound_magic_header_metadata',[(0x8c65ae44,'sa_sound_magic_header_metadata'),(0x8c10ca5c,'sa_signed_division')]),('sound_magic_extension_word',[(0x8c65af64,'sa_sound_magic_extension_word')]),('sound_magic_stream_parameters',[(0x8c65afca,'sa_sound_magic_stream_parameters')]),('engine_frame_initialize_native',[(0x8c10d31c,'sa_memory_fill')])]:
 dll=ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll'))
 for a,n in entries:lower[a]=getattr(dll,n);lower[a].argtypes=abi
assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0
for asset,bank,configured_result in itertools.product(assets,[0,0x200000],[0,0xffffffff]):
 path=ROOT/asset['path'];size=path.stat().st_size;assert size==asset['bytes']
 with path.open('rb') as file:data=file.read(64)
 assert data.hex()==asset['header_hex'] and data[:2]==b'\x80\x00'
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(data)]=data;state=0xa20000;ram[state:state+256]=bytes([0x55])*256
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in lower:return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c644be6,a;actual.append(tuple(r[i] for i in range(4,7)));r[0]=configured_result;return 0
 def hook(mem,r,sr):expected.append(tuple(r[i] for i in range(4,7)));r[0]=configured_result;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca20000,0x8ca10000,size];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c667596
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{0x8c644be6:hook},fpu=f,special=special)
 assert lower[0x8c667596](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==expected
 target=bytearray([0x55])*256;struct.pack_into('<H',target,2,1);target[12:16]=bytes([data[4],data[6],data[7],data[5]])
 n=(ctypes.c_int8(data[5]).value-2)*8;d=ctypes.c_int8(data[6]).value;q=abs(n)//abs(d)*(-1 if (n<0)!=(d<0) else 1) if d else 0
 rate,length=struct.unpack_from('>2I',data,8);struct.pack_into('<3I',target,16,q&0xffffffff,rate,length)
 offset=struct.unpack_from('>H',data,2)[0];accepted=size>=18 and offset>13
 if accepted:
  extension=struct.unpack_from('>H',data,16)[0];struct.pack_into('<H',target,0x1c,extension)
  struct.pack_into('<H',target,0x24,0)
  if size>=48 and offset>=44:
   value,flag=struct.unpack_from('>2H',data,20);struct.pack_into('<I',target,0x20,value);struct.pack_into('<H',target,0x24,flag)
   if flag==1:struct.pack_into('<H',target,0x26,struct.unpack_from('>H',data,26)[0]);struct.pack_into('<4I',target,0x28,*struct.unpack_from('>4I',data,28))
  struct.pack_into('<3I',target,0x58,ctypes.c_int8(data[7]).value&0xffffffff,ctypes.c_int8(data[5]).value&0xffffffff,q&0xffffffff)
  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
  struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<I',target,0x9c,0);struct.pack_into('<H',target,0xa8,0);struct.pack_into('<I',target,0x4c,0)
  assert actual==[(0x55555555,rate,ctypes.c_int16(extension).value&0xffffffff)]
 else:assert not actual
 assert native[state:state+256]==target,(asset['path'],[(hex(i),native[state+i],target[i]) for i in range(256) if native[state+i]!=target[i]])
 assert r[0]==(ctypes.c_int16((offset+4)&65535).value&0xffffffff if accepted else 0)
 assert native[0xa10000:0xa10000+len(data)]==data
 records.append(dict(path=asset['path'],bytes=size,header_sha256=hashlib.sha256(data).hexdigest(),bank=bank,configured_result=hex(configured_result),returned_offset=hex(r[0])))
 cases+=1
(out/'analysis/magic_retail_initializer_verification.json').write_text(json.dumps(dict(cases_passed=cases,original_files=len(assets),assets=records,scope='436 full retail RAM/CPU/FPU/SR/PR/MACL/configuration-trace comparisons across109 original magic8000 headers,banks,configuration result0/ffffffff. Native format selector->magic initializer->metadata/extension/parameters/signed division/fill. Independent full256-byte state,original64-byte headers/actual file size,metadata,signed extensions,parameter partial outcomes,copied defaults,internal pointer,format0,returned signed offset and file/rate/extension forwarding. Only configuration644be6 modeled; its error ignored as retail. Full payload/decoder/physical audio/fullgame unverified.'),indent=2)+'\n')
print(cases,'original magic audio initializer comparisons passed')
