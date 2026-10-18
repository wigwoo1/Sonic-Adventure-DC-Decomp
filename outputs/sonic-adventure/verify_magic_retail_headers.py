import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lower={}
for stem,entries in [('sound_magic_header_metadata',[(0x8c65ae44,'sa_sound_magic_header_metadata'),(0x8c10ca5c,'sa_signed_division')]),('sound_magic_extension_word',[(0x8c65af64,'sa_sound_magic_extension_word')]),('sound_magic_stream_parameters',[(0x8c65afca,'sa_sound_magic_stream_parameters')])]:
 dll=ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll'))
 for a,n in entries:lower[a]=getattr(dll,n);lower[a].argtypes=abi
assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0
for asset,bank,address in itertools.product(assets,[0,0x200000],[0x8c65ae44,0x8c65af64,0x8c65afca]):
 path=ROOT/asset['path'];size=path.stat().st_size;assert size==asset['bytes']
 with path.open('rb') as file:data=file.read(64)
 assert data.hex()==asset['header_hex'] and data[:2]==b'\x80\x00' and len(data)>=48
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(data)]=data;ram[0xa20000:0xa20040]=bytes([0x55])*64
 args=[0x8ca10000,size,0x8ca20000]
 if address==0x8c65ae44:args.append(0x8ca20004);struct.pack_into('<6I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28)])
 if address==0x8c65afca:args.append(0x8ca20004);struct.pack_into('<5I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:4+len(args)]=args;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=address
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10ca5c,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert lower[address](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 target=bytearray([0x55])*64;offset=struct.unpack_from('>H',data,2)[0];result=0
 if address==0x8c65ae44:
  struct.pack_into('<H',target,0,(offset+4)&65535);target[4]=data[4];target[8]=data[6];target[12]=data[5];target[16]=data[7]
  n=(ctypes.c_int8(data[5]).value-2)*8;d=ctypes.c_int8(data[6]).value;q=abs(n)//abs(d)*(-1 if (n<0)!=(d<0) else 1) if d else 0
  rate,length=struct.unpack_from('>2I',data,8);struct.pack_into('<3I',target,20,rate,length,q&0xffffffff)
 elif address==0x8c65af64:
  if size>=18 and offset>13:struct.pack_into('<H',target,0,struct.unpack_from('>H',data,16)[0])
  else:result=0xffffffff
 else:
  struct.pack_into('<H',target,4,0)
  if size<48 or offset<44:result=0xffffffff
  else:
   value,flag=struct.unpack_from('>2H',data,20);struct.pack_into('<I',target,0,value);struct.pack_into('<H',target,4,flag)
   if flag==1:struct.pack_into('<H',target,8,struct.unpack_from('>H',data,26)[0]);struct.pack_into('<4I',target,12,*struct.unpack_from('>4I',data,28))
   else:result=0xfffffffe
 assert native[0xa20000:0xa20040]==target and r[0]==result,(asset['path'],hex(address))
 assert native[0xa10000:0xa10000+len(data)]==data
 records.append(dict(path=asset['path'],bytes=size,header_sha256=hashlib.sha256(data).hexdigest(),bank=bank,routine=hex(address),result=hex(result)))
 cases+=1
(out/'analysis/magic_retail_headers_verification.json').write_text(json.dumps(dict(cases_passed=cases,original_files=len(assets),assets=records,scope='654 full retail RAM/CPU/FPU/SR/PR/MACL comparisons across109 original magic8000 file headers,banks,three native metadata/extension/parameter helpers. Native signed division,no modeled calls. Original64-byte header prefix and actual file size used; independent BE fields,rejection/partial outputs,offset/quotient calculation,guard/source-header preservation. Header SHA256 recorded; full payload not loaded or decoded. Native initializer integration/physical playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'original magic audio header comparisons passed')
